/**
 * @file benchmark.c
 * @author D. Heger
 * @brief Frame timing and memory instrumentation harness for the 3D cube renderer.
 * @version 1.2.0
 * @date 2025-10-04
 */

#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef __GLIBC__
#include <malloc.h>
#if !defined(HAVE_MALLINFO2)
#  if (__GLIBC__ > 2) || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 33)
#    define HAVE_MALLINFO2 1
#  endif
#endif
#endif

#include "constants.h"
#include "cube.h"
#include "renderer.h"

#define DEFAULT_RUN_SECONDS 10.0
#define MIN_RUN_SECONDS 1.0
#define DEFAULT_WARMUP_FRAMES 60
#define INITIAL_DURATION_CAPACITY 1024
#define NSEC_PER_SECOND 1000000000L
#define LOG_DEFAULT_PATH "logs/benchmark.csv"
#define ENV_RUN_SECONDS "RUN_SECONDS"
#define ENV_LOG_PATH "CUBE_BENCHMARK_LOG"

typedef struct DurationSeries {
    double* values;
    size_t size;
    size_t capacity;
} DurationSeries;

typedef struct BenchmarkConfig {
    double runSeconds;
    const char* logPath;
    size_t warmupFrames;
} BenchmarkConfig;

typedef struct BenchmarkStats {
    size_t frameCount;
    double totalMs;
    double averageMs;
    double percentile95Ms;
    double maxMs;
} BenchmarkStats;

typedef struct MemorySnapshot {
    long ruMaxRssKiB;
    long statmResidentKiB;
    long statmDataKiB;
    bool hasMallinfo;
    size_t mallinfoAllocatedKiB;
} MemorySnapshot;

/* -------------------------------------------------------------------------- */
/* Utility helpers                                                            */
/* -------------------------------------------------------------------------- */

static double clampRunSeconds(double requested)
{
    return requested < MIN_RUN_SECONDS ? MIN_RUN_SECONDS : requested;
}

static double diffMilliseconds(const struct timespec* start, const struct timespec* end)
{
    long secDelta = end->tv_sec - start->tv_sec;
    long nsecDelta = end->tv_nsec - start->tv_nsec;
    if (nsecDelta < 0) {
        --secDelta;
        nsecDelta += NSEC_PER_SECOND;
    }
    return (double)secDelta * 1000.0 + (double)nsecDelta / 1e6;
}

static double diffSeconds(const struct timespec* start, const struct timespec* end)
{
    return diffMilliseconds(start, end) / 1000.0;
}

static int compareDoubleAscending(const void* lhs, const void* rhs)
{
    const double a = *(const double*)lhs;
    const double b = *(const double*)rhs;
    if (a < b) {
        return -1;
    }
    if (a > b) {
        return 1;
    }
    return 0;
}

static bool ensureDirectoryForPath(const char* filePath)
{
    const char* separator = strrchr(filePath, '/');
    if (separator == NULL) {
        return true; /* Path resides in current directory */
    }

    size_t dirLen = (size_t)(separator - filePath);
    if (dirLen == 0) {
        return true;
    }

    char directory[PATH_MAX];
    if (dirLen >= sizeof(directory)) {
        fprintf(stderr, "[benchmark] Error: log directory path too long.\n");
        return false;
    }

    memcpy(directory, filePath, dirLen);
    directory[dirLen] = '\0';

    struct stat st = {0};
    if (stat(directory, &st) == 0) {
        return S_ISDIR(st.st_mode);
    }

    if (errno != ENOENT) {
        perror("[benchmark] stat");
        return false;
    }

    if (mkdir(directory, 0755) != 0 && errno != EEXIST) {
        perror("[benchmark] mkdir");
        return false;
    }

    return true;
}

/* -------------------------------------------------------------------------- */
/* Duration series helpers                                                    */
/* -------------------------------------------------------------------------- */

static bool durationSeriesInit(DurationSeries* series, size_t initialCapacity)
{
    series->values = (double*)malloc(initialCapacity * sizeof(double));
    if (series->values == NULL) {
        return false;
    }
    series->size = 0;
    series->capacity = initialCapacity;
    return true;
}

static void durationSeriesFree(DurationSeries* series)
{
    free(series->values);
    series->values = NULL;
    series->size = 0;
    series->capacity = 0;
}

static bool durationSeriesAppend(DurationSeries* series, double value)
{
    if (series->size == series->capacity) {
        size_t newCapacity = series->capacity * 2;
        double* resized = (double*)realloc(series->values, newCapacity * sizeof(double));
        if (resized == NULL) {
            return false;
        }
        series->values = resized;
        series->capacity = newCapacity;
    }

    series->values[series->size++] = value;
    return true;
}

static double calculatePercentile(DurationSeries* series, double percentile)
{
    if (series->size == 0) {
        return 0.0;
    }

    qsort(series->values, series->size, sizeof(double), compareDoubleAscending);

    double position = percentile / 100.0 * (double)(series->size - 1);
    size_t lowerIndex = (size_t)floor(position);
    size_t upperIndex = (size_t)ceil(position);
    double fraction = position - (double)lowerIndex;

    double result = series->values[lowerIndex];
    if (upperIndex != lowerIndex) {
        result += (series->values[upperIndex] - series->values[lowerIndex]) * fraction;
    }

    return result;
}

/* -------------------------------------------------------------------------- */
/* Memory instrumentation                                                     */
/* -------------------------------------------------------------------------- */

static void readProcStatm(long* residentKiB, long* dataKiB)
{
    if (residentKiB != NULL) {
        *residentKiB = -1;
    }
    if (dataKiB != NULL) {
        *dataKiB = -1;
    }

    FILE* fp = fopen("/proc/self/statm", "r");
    if (fp == NULL) {
        return;
    }

    long size = 0L;
    long resident = 0L;
    long share = 0L;
    long text = 0L;
    long lib = 0L;
    long data = 0L;
    long dt = 0L;

    if (fscanf(fp,
               "%ld %ld %ld %ld %ld %ld %ld",
               &size,
               &resident,
               &share,
               &text,
               &lib,
               &data,
               &dt) == 7) {
        const long pageSizeBytes = sysconf(_SC_PAGESIZE);
        const long pageSizeKiB = pageSizeBytes > 0 ? pageSizeBytes / 1024 : 4;
        if (residentKiB != NULL) {
            *residentKiB = resident * pageSizeKiB;
        }
        if (dataKiB != NULL) {
            *dataKiB = data * pageSizeKiB;
        }
    }

    fclose(fp);
}

static void captureMemorySnapshot(MemorySnapshot* snapshot)
{
    memset(snapshot, 0, sizeof(*snapshot));
    snapshot->ruMaxRssKiB = -1;
    snapshot->statmResidentKiB = -1;
    snapshot->statmDataKiB = -1;
    snapshot->hasMallinfo = false;
    snapshot->mallinfoAllocatedKiB = 0U;

    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        snapshot->ruMaxRssKiB = usage.ru_maxrss;
    }

    readProcStatm(&snapshot->statmResidentKiB, &snapshot->statmDataKiB);

#if defined(HAVE_MALLINFO2)
    struct mallinfo2 info = mallinfo2();
    snapshot->mallinfoAllocatedKiB = (size_t)(info.uordblks / 1024U);
    snapshot->hasMallinfo = true;
#elif defined(__GLIBC__)
    struct mallinfo legacy = mallinfo();
    snapshot->mallinfoAllocatedKiB = (size_t)(legacy.uordblks / 1024U);
    snapshot->hasMallinfo = true;
#endif
}

/* -------------------------------------------------------------------------- */
/* Benchmark configuration and execution                                     */
/* -------------------------------------------------------------------------- */

static double parseRunSeconds(void)
{
    const char* envValue = getenv(ENV_RUN_SECONDS);
    if (envValue == NULL || envValue[0] == '\0') {
        return DEFAULT_RUN_SECONDS;
    }

    char* endPtr = NULL;
    errno = 0;
    double parsed = strtod(envValue, &endPtr);

    if (errno != 0 || endPtr == envValue || !isfinite(parsed)) {
        fprintf(stderr,
                "[benchmark] Warning: invalid %s value '%s'. Using default %.2f seconds.\n",
                ENV_RUN_SECONDS,
                envValue,
                DEFAULT_RUN_SECONDS);
        return DEFAULT_RUN_SECONDS;
    }

    return clampRunSeconds(parsed);
}

static BenchmarkConfig loadBenchmarkConfig(void)
{
    BenchmarkConfig config;
    config.runSeconds = parseRunSeconds();

    const char* logPathOverride = getenv(ENV_LOG_PATH);
    config.logPath = (logPathOverride != NULL && logPathOverride[0] != '\0')
                         ? logPathOverride
                         : LOG_DEFAULT_PATH;

    config.warmupFrames = DEFAULT_WARMUP_FRAMES;
    return config;
}

static void logConfigSummary(const BenchmarkConfig* config)
{
    printf("[benchmark] Warmup frames : %zu\n", config->warmupFrames);
    printf("[benchmark] Measure time  : %.2f seconds (minimum %.1f)\n",
           config->runSeconds,
           MIN_RUN_SECONDS);
    printf("[benchmark] CSV output    : %s\n", config->logPath);
}

static bool renderSingleFrame(void)
{
    if (initializeFrameBuffer() != ALLOCATION_SUCCESS ||
        initializeDepthBuffer() != ALLOCATION_SUCCESS) {
        fprintf(stderr, "[benchmark] Error: buffer initialization failed.\n");
        return false;
    }

    drawCube(rotationAngles);
    incrementRotationAngles();
    return true;
}

static bool measureFrame(double* frameMs)
{
    struct timespec frameStart;
    struct timespec frameEnd;

    if (clock_gettime(CLOCK_MONOTONIC, &frameStart) != 0) {
        perror("[benchmark] clock_gettime");
        return false;
    }

    if (!renderSingleFrame()) {
        return false;
    }

    if (clock_gettime(CLOCK_MONOTONIC, &frameEnd) != 0) {
        perror("[benchmark] clock_gettime");
        return false;
    }

    *frameMs = diffMilliseconds(&frameStart, &frameEnd);
    return true;
}

static bool warmupRenderer(size_t warmupFrames)
{
    for (size_t frame = 0; frame < warmupFrames; ++frame) {
        if (!renderSingleFrame()) {
            return false;
        }
    }
    return true;
}

static bool runBenchmark(const BenchmarkConfig* config,
                         DurationSeries* timings,
                         BenchmarkStats* stats)
{
    struct timespec phaseStart;
    if (clock_gettime(CLOCK_MONOTONIC, &phaseStart) != 0) {
        perror("[benchmark] clock_gettime");
        return false;
    }

    while (true) {
        struct timespec now;
        if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) {
            perror("[benchmark] clock_gettime");
            return false;
        }

        double elapsedSeconds = diffSeconds(&phaseStart, &now);
        if (elapsedSeconds >= config->runSeconds && stats->frameCount > 0) {
            break;
        }

        double frameMs = 0.0;
        if (!measureFrame(&frameMs)) {
            return false;
        }

        if (!durationSeriesAppend(timings, frameMs)) {
            fprintf(stderr, "[benchmark] Error: unable to grow duration buffer.\n");
            return false;
        }

        stats->frameCount += 1;
        stats->totalMs += frameMs;
        if (frameMs > stats->maxMs) {
            stats->maxMs = frameMs;
        }
    }

    if (stats->frameCount == 0) {
        fprintf(stderr, "[benchmark] Error: no frames rendered during measurement.\n");
        return false;
    }

    stats->averageMs = stats->totalMs / (double)stats->frameCount;
    stats->percentile95Ms = calculatePercentile(timings, 95.0);
    return true;
}

static bool appendCsvRow(const char* csvPath,
                         const char* timestamp,
                         const BenchmarkStats* stats,
                         const MemorySnapshot* memory)
{
    if (!ensureDirectoryForPath(csvPath)) {
        return false;
    }

    bool needHeader = access(csvPath, F_OK) != 0;
    FILE* file = fopen(csvPath, "a");
    if (file == NULL) {
        perror("[benchmark] fopen");
        return false;
    }

    if (needHeader) {
        fprintf(file,
                "timestamp,frames,total_ms,avg_ms,p95_ms,max_ms,max_rss_kib\n");
    }

    fprintf(file,
            "%s,%zu,%.3f,%.3f,%.3f,%.3f,%ld\n",
            timestamp,
            stats->frameCount,
            stats->totalMs,
            stats->averageMs,
            stats->percentile95Ms,
            stats->maxMs,
            memory->ruMaxRssKiB);

    fclose(file);
    return true;
}

static void formatIsoTimestamp(char* buffer, size_t bufferSize)
{
    time_t rawTime = time(NULL);
    struct tm timeInfo;
    gmtime_r(&rawTime, &timeInfo);
    strftime(buffer, bufferSize, "%Y-%m-%dT%H:%M:%SZ", &timeInfo);
}

/* -------------------------------------------------------------------------- */
/* Entry point                                                                */
/* -------------------------------------------------------------------------- */

int main(void)
{
    BenchmarkConfig config = loadBenchmarkConfig();
    logConfigSummary(&config);

    if (initializeRenderer(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT) != ALLOCATION_SUCCESS) {
        fprintf(stderr, "[benchmark] Error: renderer initialization failed.\n");
        return EXIT_FAILURE;
    }

    DurationSeries timings = {0};
    bool timingsInitialized = false;

    if (!durationSeriesInit(&timings, INITIAL_DURATION_CAPACITY)) {
        fprintf(stderr, "[benchmark] Error: unable to allocate timing buffer.\n");
        cleanupRenderer();
        return EXIT_FAILURE;
    }
    timingsInitialized = true;

    if (!warmupRenderer(config.warmupFrames)) {
        if (timingsInitialized) {
            durationSeriesFree(&timings);
        }
        cleanupRenderer();
        return EXIT_FAILURE;
    }

    MemorySnapshot beforeSnapshot;
    captureMemorySnapshot(&beforeSnapshot);

    BenchmarkStats stats = {0};
    if (!runBenchmark(&config, &timings, &stats)) {
        if (timingsInitialized) {
            durationSeriesFree(&timings);
        }
        cleanupRenderer();
        return EXIT_FAILURE;
    }

    MemorySnapshot afterSnapshot;
    captureMemorySnapshot(&afterSnapshot);

    char isoTimestamp[32];
    formatIsoTimestamp(isoTimestamp, sizeof(isoTimestamp));

    printf("\nBenchmark Summary (%s)\n", isoTimestamp);
    printf("  Frames Rendered      : %zu\n", stats.frameCount);
    printf("  Total Frame Time     : %.3f ms\n", stats.totalMs);
    printf("  Average Frame Time   : %.3f ms\n", stats.averageMs);
    printf("  95th Percentile      : %.3f ms\n", stats.percentile95Ms);
    printf("  Max Frame Time       : %.3f ms\n", stats.maxMs);
    printf("  Max RSS (before)     : %ld KiB\n", beforeSnapshot.ruMaxRssKiB);
    printf("  Max RSS (after)      : %ld KiB\n", afterSnapshot.ruMaxRssKiB);
    printf("  RSS delta            : %ld KiB\n",
           afterSnapshot.ruMaxRssKiB - beforeSnapshot.ruMaxRssKiB);

    if (afterSnapshot.statmResidentKiB >= 0) {
        printf("  Resident Set (statm) : %ld KiB\n", afterSnapshot.statmResidentKiB);
    }
    if (afterSnapshot.statmDataKiB >= 0) {
        printf("  Data Segment (statm) : %ld KiB\n", afterSnapshot.statmDataKiB);
    }
    if (afterSnapshot.hasMallinfo) {
        printf("  Heap Allocated       : %zu KiB\n", afterSnapshot.mallinfoAllocatedKiB);
    } else {
        printf("  Heap Allocated       : (mallinfo unavailable on this platform)\n");
    }

    if (!appendCsvRow(config.logPath, isoTimestamp, &stats, &afterSnapshot)) {
        fprintf(stderr, "[benchmark] Warning: failed to append CSV row.\n");
    } else {
        printf("  CSV Output           : %s\n", config.logPath);
    }

    if (timingsInitialized) {
        durationSeriesFree(&timings);
    }
    cleanupRenderer();
    return EXIT_SUCCESS;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>

#define MAX_DEVICES 100
#define MAX_TOWERS 50

// Структуры данных
typedef struct {
    char device_id[20];
    float signal_strength;
    int connected_cells[5];
    float throughput_mbps;
    char technology[5]; 
} MobileDevice;

typedef struct {
    int cell_id;
    float frequency_ghz;
    float max_bandwidth;
    int active_users;
    float load_percentage;
    char cell_type[10]; 
} CellTower;

typedef struct {
    float dl_speed;
    float ul_speed;
    int latency_ms;
    int packet_loss;
} NetworkMetrics;

// Глобальные переменные
MobileDevice devices[MAX_DEVICES];
CellTower towers[MAX_TOWERS];
int device_count = 0;
int tower_count = 0;

// ============= ФУНКЦИИ УПРАВЛЕНИЯ СЕТЬЮ =============

void register_device(char *device_id, char *technology) {
    if (device_count >= MAX_DEVICES) {
        printf("Maximum devices reached!\n");
        return;
    }
    
    // ИСПРАВЛЕНО: strncpy вместо strcpy для защиты от переполнения
    strncpy(devices[device_count].device_id, device_id, 19);
    devices[device_count].device_id[19] = '\0';
    
    strncpy(devices[device_count].technology, technology, 4);
    devices[device_count].technology[4] = '\0';
    
    devices[device_count].signal_strength = 0;
    devices[device_count].throughput_mbps = 0;
    
    // ИСПРАВЛЕНО: цикл до < 5 (индексы 0-4), раньше было <= 5 (выход за границу)
    for (int i = 0; i < 5; i++) {
        devices[device_count].connected_cells[i] = -1;
    }
    
    device_count++;
}

void add_cell_tower(int cell_id, float frequency, float bandwidth, char *type) {
    if (tower_count >= MAX_TOWERS) return;
    
    towers[tower_count].cell_id = cell_id;
    towers[tower_count].frequency_ghz = frequency;
    towers[tower_count].max_bandwidth = bandwidth;
    towers[tower_count].active_users = 0;
    
    strncpy(towers[tower_count].cell_type, type, 9);
    towers[tower_count].cell_type[9] = '\0';
    
    towers[tower_count].load_percentage = 0;
    tower_count++;
}

float calculate_signal_strength(float distance_km, float frequency_ghz) {
    // ИСПРАВЛЕНО: защита от бесконечной рекурсии и log10(0)
    if (distance_km <= 0.001) { 
        distance_km = 0.001; 
    }
    
    float loss = 20 * log10(distance_km) + 20 * log10(frequency_ghz) + 32.44;
    float signal = 100 - loss;
    
    // ИСПРАВЛЕНО: инициализация переменной по умолчанию
    float normalized = 0;
    if (signal > 0) {
        normalized = signal / 100;
    }
    if (normalized > 1.0) normalized = 1.0; // Ограничение сверху
    
    return normalized * 100;
}

// ИСПРАВЛЕНО: функция теперь принимает размер буфера, чтобы избежать Heap Overflow
void get_device_info(MobileDevice *device, char *out_buffer, size_t buf_size) {
    if (!device || !out_buffer) return;
    
    snprintf(out_buffer, buf_size, "Device: %s, Tech: %s, Signal: %.1f, Speed: %.1f", 
             device->device_id, device->technology, 
             device->signal_strength, device->throughput_mbps);
}

void calculate_network_metrics(NetworkMetrics *metrics, int device_idx) {
    // ИСПРАВЛЕНО: проверка индекса (должен быть < device_count)
    if (device_idx < 0 || device_idx >= device_count) {
        metrics->dl_speed = 0;
        metrics->ul_speed = 0;
        metrics->latency_ms = 0;
        metrics->packet_loss = 0;
        return;
    }
    
    MobileDevice *device = &devices[device_idx];
    
    if (strcmp(device->technology, "5G") == 0) {
        metrics->dl_speed = device->signal_strength * 10;
        metrics->ul_speed = metrics->dl_speed / 4;
        metrics->latency_ms = 20 - (int)(device->signal_strength / 10);
    } else {
        metrics->dl_speed = device->signal_strength * 2;
        metrics->ul_speed = metrics->dl_speed / 5;
        metrics->latency_ms = 50 - (int)(device->signal_strength / 5);
    }
    
    if (metrics->latency_ms < 1) metrics->latency_ms = 1;
    metrics->packet_loss = 0;
}

void handover_device(int device_index, int from_cell, int to_cell) {
    if (device_index < 0 || device_index >= device_count) return;
    
    MobileDevice *device = &devices[device_index];
    
    int from_index = -1, to_index = -1;
    // ИСПРАВЛЕНО: цикл < tower_count (раньше было <=)
    for (int i = 0; i < tower_count; i++) {
        if (towers[i].cell_id == from_cell) from_index = i;
        if (towers[i].cell_id == to_cell) to_index = i;
    }
    
    if (from_index != -1 && towers[from_index].active_users > 0) {
        towers[from_index].active_users--;
    }
    
    if (to_index != -1) {
        towers[to_index].active_users++;
    }
    
    for (int i = 0; i < 5; i++) {
        if (device->connected_cells[i] == from_cell) {
            device->connected_cells[i] = to_cell;
            break;
        }
    }
}

// ИСПРАВЛЕНО: Возврат статической строки или использование буфера. 
// Раньше функция возвращала адрес локального массива, который удалялся.
const char* get_cell_status(int cell_id, char *buffer) {
    sprintf(buffer, "Cell %d is active", cell_id);
    return buffer;
}

void add_network_slice(char *slice_name) {
    static char **slices = NULL;
    static int slice_count = 0;
    
    // ИСПРАВЛЕНО: realloc должен учитывать размер типа (sizeof(char*))
    char **temp_slices = realloc(slices, (slice_count + 1) * sizeof(char*));
    if (temp_slices == NULL) return;
    slices = temp_slices;
    
    slices[slice_count] = malloc(strlen(slice_name) + 1);
    if (slices[slice_count]) {
        strcpy(slices[slice_count], slice_name);
        slice_count++;
    }
    // Примечание: в реальной программе нужно предусмотреть очистку slices в конце
}

// ============= ТЕСТЫ =============

int main() {
    printf("=== Starting Simulator ===\n");
    
    register_device("iPhone12", "4G");
    register_device("GalaxyS21", "5G");
    
    // ИСПРАВЛЕНО: корректный вызов силы сигнала (не 0)
    devices[0].signal_strength = calculate_signal_strength(0.5, 1.8);
    
    add_cell_tower(101, 1.8, 100, "macro");
    add_cell_tower(102, 3.5, 400, "pico");
    
    devices[0].connected_cells[0] = 101;
    handover_device(0, 101, 102);
    
    // ИСПРАВЛЕНО: Указатель теперь инициализирован
    MobileDevice *device_ptr = &devices[0];
    printf("Device technology: %s\n", device_ptr->technology);
    
    // ИСПРАВЛЕНО: Работа с буфером для информации об устройстве
    char info_buf[100];
    get_device_info(&devices[0], info_buf, sizeof(info_buf));
    printf("%s\n", info_buf);
    
    // ИСПРАВЛЕНО: Удалено двойное освобождение памяти
    char *buffer = malloc(100);
    if (buffer) {
        free(buffer);
        buffer = NULL; // Хорошая практика
    }
    
    // ИСПРАВЛЕНО: Работа со статусом через буфер
    char status_buf[50];
    printf("Status: %s\n", get_cell_status(101, status_buf));
    
    // ИСПРАВЛЕНО: Безопасное копирование в маленький буфер
    char small_buffer[10];
    strncpy(small_buffer, "Too long string", sizeof(small_buffer) - 1);
    small_buffer[sizeof(small_buffer) - 1] = '\0';

    printf("Done without crashes!\n");
    return 0;
}
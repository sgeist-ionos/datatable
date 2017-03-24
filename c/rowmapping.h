#ifndef dt_ROWMAPPING_H
#define dt_ROWMAPPING_H
#include "datatable.h"

typedef enum RowMappingType {
    RI_ARRAY,
    RI_SLICE,
} RowMappingType;


typedef struct RowMapping {
    RowMappingType type;
    int64_t length;
    union {
        int64_t* indices;
        struct { int64_t start, step; } slice;
    };
} RowMapping;

typedef int64_t (*filter_fn)(DataTable*, int64_t*, int64_t, int64_t);

RowMapping* RowMapping_from_slice(int64_t start, int64_t count, int64_t step);
RowMapping* RowMapping_from_array(int64_t* array, int64_t length);
RowMapping* RowMapping_from_filter(DataTable* dt, filter_fn filter);
RowMapping* RowMapping_merge(RowMapping *risrc, RowMapping *rinew);
void RowMapping_dealloc(RowMapping *rowmapping);


#endif

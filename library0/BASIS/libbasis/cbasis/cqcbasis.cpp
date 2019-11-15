#include "cqcbasis.h"
#include <cassert>
#include <cfloat>
#include <cmath>
#include <string>
#include <vector>

//data:

void _cq_assign_data(_cq_data *data, const void *items, size_t size, size_t count) {
    if (data == nullptr) {
        return;
    }
    
    if (items != nullptr && size > 0 && count > 0) {
        _cq_resize_data(data, size, count);
        memcpy(data->items, items, size * count);
    } else {
        _cq_clear_data(data);
    }
}

void _cq_clear_data(_cq_data *data) {
    if (data != nullptr) {
        free(data->items);
        data->items = nullptr;
        data->size  = 0;
        data->count = 0;
    }
}

void _cq_resize_data(_cq_data *data, size_t size, size_t count) {
    if (data == nullptr) {
        return;
    }
    
    if (size > 0 && count > 0) {
        {
            //think that realloc() always successful. the use of assert() is just to
            //suppress a warning on visual c++ default configuration.
            void *items = realloc(data->items, size * (count + 1));
            assert(items != nullptr);
            data->items = items;
        }
        memcpy((char *)data->items + size * count, "\0\0\0\0\0\0\0\0", size);
        data->size = size;
        data->count = count;
    } else {
        _cq_clear_data(data);
    }
}

//float number comparision

bool cq_flt_equal(float a, float b) {
    return fabsf(a - b) < FLT_EPSILON;
}

bool cq_dbl_equal(double a, double b) {
    return fabs(a - b) < DBL_EPSILON;
}

//string:

bool cq_str_empty   (const char     *s) {return s == nullptr || *s == '\0';}
bool cq_u16str_empty(const char16_t *s) {return s == nullptr || *s == '\0';}

template<class T> T *cq_copy_str(const T *src) {
    if (src != nullptr) {
        size_t len = std::char_traits<T>::length(src);
        T *dst = (T *)malloc(sizeof(T) * (len + 1));
        
        //think that malloc() always successful. the use of assert() is just to
        //suppress a warning on visual c++ default configuration.
        assert(dst != nullptr);
        
        std::char_traits<T>::copy(dst, src, len);
        dst[len] = '\0';
        return dst;
    }
    return nullptr;
}

char     *cq_copy_str   (const char     *s) {return cq_copy_str<char    >(s);}
char16_t *cq_copy_u16str(const char16_t *s) {return cq_copy_str<char16_t>(s);}

template<class T> const T *cq_store_str(const T *string) {
    static thread_local _cq_data store = {nullptr, 0, 0};
    
    size_t count = 0;
    if (string != nullptr) {
        count = std::char_traits<T>::length(string);
    }
    _cq_assign_data(&store, string, sizeof(T), count);
    return (const T *)store.items;
}

const char     *cq_store_str   (const char     *s) {return cq_store_str<char    >(s);}
const char16_t *cq_store_u16str(const char16_t *s) {return cq_store_str<char16_t>(s);}

//unicode:

static char32_t u32c_from8s(const _char8_t *ptr, size_t *count) {
    char32_t ch = 0;

    //utf-8 first byte:
    //
    // 1 byte  occupied,  7 valid bits: 0xxx'xxxx
    // 2 bytes occupied, 11 valid bits: 110x'xxxx 10xx xxxx ...
    // 3 bytes occupied, 16 valid bits: 1110'xxxx 10xx xxxx ...
    // 4 bytes occupied, 21 valid bits: 1111'0xxx 10xx xxxx ...
    //
    if /**/ ((*ptr & 0b10'00'0000) == 0b00'00'0000) { *count = 1; ch = *ptr & 0b01'11'1111;}
    else if ((*ptr & 0b11'10'0000) == 0b11'00'0000) { *count = 2; ch = *ptr & 0b00'01'1111;}
    else if ((*ptr & 0b11'11'0000) == 0b11'10'0000) { *count = 3; ch = *ptr & 0b00'00'1111;}
    else if ((*ptr & 0b11'11'1000) == 0b11'11'0000) { *count = 4; ch = *ptr & 0b00'00'0111;}
    else {
        *count = 0;
        return 0;
    }
    
    //followed bytes: 10xx'xxxx
    for (const _char8_t *it = ptr + 1; it < ptr + *count; ++it) {
        if ((*it & 0b11'00'0000) != 0b10'00'0000) {
            *count = 0;
            return 0;
        }
        ch <<= 6;
        ch |= (*it & 0b01'11'1111);
    }
    
    return ch;
}

struct u16encoded {
    char16_t item[4];
};

static u16encoded u16e_from32c(char32_t src) {
    u16encoded dst;
    memset(&dst, 0, sizeof(dst));
    
    //utf-16 surrogate pair (4 bytes):
    //
    //  1st 2 bytes: 1101 10 xx,xxxx xxxx
    //  2nd 2 bytes: 1101 11 xx,xxxx xxxx
    //
    if (src > 0xffFF) {
        
        //the biggest point of unicode is 0x10FFff (the last of 16st plane),
        //when a point subtracted from 0x10000, the remaining value will not exceed 20 bit.
        src -= 0x10000;
        dst.item[0] = ((src >> 10) & 0x3ff) | 0xd800;
        dst.item[1] = ((src      ) & 0x3ff) | 0xdc00;
    }
    //BMP character, 2 bytes.
    else {
        dst.item[0] = (char16_t)src;
    }
    
    return dst;
}

const char16_t *cq_u16s_from8s(const _char8_t *src) {
    if (src == nullptr) {
        return cq_store_u16str(nullptr);
    }
    
    std::u16string dst;
    while (true) {
        size_t count = 0;
        char32_t point = u32c_from8s(src, &count);
        if (count > 0) {
            if (point != '\0') {
                dst.append(u16e_from32c(point).item);
                src += count;
            } else {
                //string end
                break;
            }
        } else {
            //error happened.
            //don't call dst.clear(), keep the available part.
            break;
        }
    }
    return cq_store_u16str(dst.c_str());
}

static char32_t u32c_from16s(const char16_t *ptr, size_t *count) {
    
    //utf-16 surrogate pair (4 bytes):
    //
    //  1st 2 bytes: 1101 10 xx,xxxx xxxx
    //  2nd 2 bytes: 1101 11 xx,xxxx xxxx
    //
    if (/**/0xd800 <= ptr[0] && ptr[0] <= 0xdbff) {
        if (0xdc00 <= ptr[1] && ptr[1] <= 0xdfff) {
            *count = 2;
            char32_t ch = 0;
            ch = (char32_t)(ptr[0] & 0x03ff);
            ch = (char32_t)(ptr[1] & 0x03ff) | (ch << 10);
            return ch + 0x10000;
        } else {
            //error happened
            *count = 0;
            return 0;
        }
    }
    //BMP character, 2 bytes.
    else {
        *count = 1;
        return (char32_t)*ptr;
    }
}

struct u8encoded {
    _char8_t item[8];
};

static u8encoded u8e_from32c(char32_t src) {
    u8encoded dst;
    memset(&dst, 0, sizeof(dst));
    
    if (src <= 0x7f) {
        
        //up to 7 bit, occupy 1 byte.
        dst.item[0] = (_char8_t)src;
        
    } else if (src <= 0x7ff) {
        
        //up to 11 bit, occupy 2 bytes.
        dst.item[0] = ((src >> 6) & 0b00'01'1111) | 0b11'00'0000;
        dst.item[1] = ((src     ) & 0b00'11'1111) | 0b10'00'0000;
        
    } else if (src <= 0xffFF) {
        
        //up to 16 bit, occupy 3 bytes.
        dst.item[0] = ((src >> 12) & 0b00'00'1111) | 0b11'10'0000;
        dst.item[1] = ((src >>  6) & 0b00'11'1111) | 0b10'00'0000;
        dst.item[2] = ((src      ) & 0b00'11'1111) | 0b10'00'0000;
        
    } else if (src <= 0x10FFff) {
        
        //up to 21 bit, occupy 4 bytes.
        dst.item[0] = ((src >> 18) & 0b00'00'0111) | 0b11'11'0000;
        dst.item[1] = ((src >> 12) & 0b00'11'1111) | 0b10'00'0000;
        dst.item[2] = ((src >>  6) & 0b00'11'1111) | 0b10'00'0000;
        dst.item[3] = ((src      ) & 0b00'11'1111) | 0b10'00'0000;
    }
    
    return dst;
}

const _char8_t *cq_u8s_from16s(const char16_t *src) {
    if (src == nullptr) {
        return cq_store_str(nullptr);
    }
    
    std::string dst;
    while (true) {
        size_t count = 0;
        char32_t point = u32c_from16s(src, &count);
        if (count > 0) {
            if (point != '\0') {
                dst.append(u8e_from32c(point).item);
                src += count;
            } else {
                //string end
                break;
            }
        } else {
            //error happened.
            //don't call dst.clear(), keep the available part.
            break;
        }
    }
    return cq_store_str(dst.c_str());
}
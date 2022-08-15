//
// Created by Administrator on 2022/8/5.
//
#include <format.h>

char ascii_int_index[] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70};

static int is_specifier(char chr);

static int is_length_tag(const char chr);

static int is_int(char cha);

static int str2int(const char *chr, size_t *value);

void format_init(const char *format, format_info_t *format_info) {
    char *format_star = format;
    format_info->length = 0;
    format_info->width = 0;
    while ((*format)) {
        if (is_specifier(*format)) {
            format_info->specifier = *format;
            format_info->format_size = format - format_star;
            break;
        }

        if (is_length_tag(*format)) {
            format_info->length++;
        }
        //如果是数字，说明开始是宽度了
        if (is_int(*format)) {
            int int_len = str2int(format, &format_info->width);
            format += int_len;
            continue;
        }
        format++;
    }

    //长度固定为最长可能
    format_info->size = 66;

}


size_t format(uint64_t src_r, char *results, const format_info_t *format_info) {
    //有正负之分的格式
    if (format_info->specifier == FORMAT_INT_BASE_10) {
        //判断正负
        if ((src_r & (((uint64_t) 1) << 63)) != 0) {
            *results = '-';
            results++;
            //对负数取反
            src_r = ~src_r+1;
        }
    }
    uint64_t  src = (uint64_t)src_r;
    //如果是0 直接返回
    if (src == 0 && format_info->width == 0) {
        results[0] = ascii_int_index[0];
        results[1] = '\0';
        return 1;
    }
    int into = 10;
    if(format_info->specifier ==FORMAT_INT_BASE_10 || format_info->specifier ==FORMAT_UINT_BASE_10 ){
        into = 10;
    } else if (format_info->specifier ==FORMAT_UINT_BASE_8){
        into = 8;
    } else if (format_info->specifier ==FORMAT_UINT_BASE_16){
        into = 16;
    }

    uint32_t str_index;
    for (str_index = 0; src > 0; str_index++) {
        int index = src % into;
        results[str_index] = ascii_int_index[index];
        src /= into;
    }

    //前置位补0;
    if(format_info->width > str_index){
        for (; str_index < format_info->width; str_index++) {
            results[str_index] = ascii_int_index[0];
        }
    }

    //反转字符串
    for (uint32_t i = 0; i < str_index / 2; ++i) {
        char t = results[i];
        results[i] = results[str_index - i - 1];
        results[str_index - i - 1] = t;
    }
    results[str_index] = '\0';

    return str_index;
}


static int is_length_tag(const char chr) {
    if (chr == FORMAT_LENGTH_TAG) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * 将字符转换为int，
 * @param chr
 * @param value  转换结果
 * @return 转换的长度
 */
static int str2int(const char *chr, size_t *value) {
    int int_len = 0;
    *value = 0;
    while ((*chr)) {
        if (is_int(*chr)) {
            int_len++;
            *value = *value * 10 + (*chr - 48);

        } else {
            break;
        }
        chr++;
    }
    return int_len;

}


static int is_int(const char chr) {
    if (chr >= 48 && chr <= 57) {
        return 1;
    } else {
        return 0;
    }
}


static int is_specifier(const char chr) {
    if (chr == FORMAT_INT_BASE_10 ||
        chr == FORMAT_UINT_BASE_8 ||
        chr == FORMAT_UINT_BASE_16 ||
        chr == FORMAT_UINT_BASE_10 ||
        chr == FORMAT_STR ||
        chr == FORMAT_CHAR
            ) {
        return 1;
    } else {
        return 0;
    }
}
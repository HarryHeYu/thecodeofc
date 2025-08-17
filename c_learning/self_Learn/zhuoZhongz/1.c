63%
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <zlib.h>

#define BUFFER_SIZE 4096
#define MAX_STR_LEN 256

// 辅助函数：跳转到间接对象
static void goto_indirect_object(FILE* pdf, int obj_num) {
    char buffer[BUFFER_SIZE];
    fseek(pdf, 0, SEEK_SET);
    while (fgets(buffer, BUFFER_SIZE, pdf)) {
        int current_num;
        if (sscanf(buffer, "%d 0 obj", &current_num) == 1 && current_num == obj_num) {
            fseek(pdf, -strlen(buffer), SEEK_CUR);
            break;
        }
    }
}

// 辅助函数：提取字符串值（处理括号和转义）
static char* extract_string_value(char* start) {
    char* end;
    if (*start == '(') {  // 处理(....)字符串
        start++;
        end = strchr(start, ')');
    } else if (*start == '<') {  // 处理<....>十六进制字符串
        start++;
        end = strchr(start, '>');
    } else {
        return NULL;
    }
    if (!end) return NULL;

    static char value[MAX_STR_LEN];
    int idx = 0;
    for (char* p = start; p < end && idx < MAX_STR_LEN-1; p++) {
        if (*p == '\\' && p+1 < end) {
            p++;
            if (isdigit(*p)) {  // 处理八进制转义
                int oct = 0;
                for (int i = 0; i < 3 && isdigit(*p); i++) {
                    oct = oct * 8 + (*p - '0');
                    p++;
                }
                p--;
                value[idx++] = (char)oct;
            } else {  // 处理简单转义
                value[idx++] = *p;
            }
        } else {
            value[idx++] = *p;
        }
    }
    value[idx] = '\0';
    return value;
}

// 改进的元数据获取函数
static bool get_metadata(FILE* pdf, const char* key, char* value) {
    char buffer[BUFFER_SIZE];
    long start_pos = ftell(pdf);
    fseek(pdf, 0, SEEK_SET);

    // 查找文档信息字典（可能为间接对象）
    int info_obj_num = -1;
    while (fgets(buffer, BUFFER_SIZE, pdf)) {
        if (strstr(buffer, "/Info")) {
            if (sscanf(buffer, "/Info %d 0 R", &info_obj_num) == 1) {
                goto_indirect_object(pdf, info_obj_num);
                break;
            }
        }
    }

    // 解析字典内容
    while (fgets(buffer, BUFFER_SIZE, pdf)) {
        char* p = strstr(buffer, key);
        if (p) {
            p += strlen(key);
            while (*p && (isspace(*p) || *p == '/')) p++;
            char* val = extract_string_value(p);
            if (val) {
                strncpy(value, val, MAX_STR_LEN);
                fseek(pdf, start_pos, SEEK_SET);
                return true;
            }
        }
        if (strstr(buffer, ">>")) break; // 字典结束
    }

    fseek(pdf, start_pos, SEEK_SET);
    return false;
}

// 元数据获取函数
bool pdf_get_creator(FILE* pdf, char* creator) {
    return get_metadata(pdf, "/Creator", creator);
}

bool pdf_get_author(FILE* pdf, char* author) {
    return get_metadata(pdf, "/Author", author);
}

bool pdf_get_producer(FILE* pdf, char* producer) {
    return get_metadata(pdf, "/Producer", producer);
}

bool pdf_get_file_title(FILE* pdf, char* file_title) {
    return get_metadata(pdf, "/Title", file_title);
}

bool pdf_get_keywords(FILE* pdf, char* keywords) {
    return get_metadata(pdf, "/Keywords", keywords);
}

// 获取页数
int pdf_get_page_num(FILE* pdf) {
    char buffer[BUFFER_SIZE];
    long pos = ftell(pdf);
    int pages = 0;
    bool found = false;

    fseek(pdf, 0, SEEK_SET);
    while (fgets(buffer, BUFFER_SIZE, pdf) && !found) {
        if (strstr(buffer, "/Type /Pages")) {
            while (fgets(buffer, BUFFER_SIZE, pdf)) {
                char* p = strstr(buffer, "/Count");
                if (p) {
                    sscanf(p, "/Count %d", &pages);
                    found = true;
                    break;
                }
                if (strstr(buffer, ">>")) break;
            }
        }
    }
    fseek(pdf, pos, SEEK_SET);
    return pages;
}

// 获取页面文本内容
bool pdf_get_page_txt(FILE* pdf, int page_num, char* page_txt) {
    char buffer[BUFFER_SIZE];
    long pos = ftell(pdf);
    int cur_page = -1;
    bool found = false;

    fseek(pdf, 0, SEEK_SET);
    while (fgets(buffer, BUFFER_SIZE, pdf) && !found) {
        if (strstr(buffer, "/Type /Page")) {
            if (++cur_page == page_num) {
                // 定位内容流
                while (fgets(buffer, BUFFER_SIZE, pdf)) {
                    if (strstr(buffer, "stream")) {
                        // 跳过可能存在的长度声明
                        while (fgets(buffer, BUFFER_SIZE, pdf) && 
                              !strstr(buffer, "stream")) ;

                        // 初始化zlib流
                        z_stream zlib_stream;
                        memset(&zlib_stream, 0, sizeof(z_stream));
                        inflateInit(&zlib_stream);

                        unsigned char in_buf[BUFFER_SIZE];
                        unsigned char out_buf[BUFFER_SIZE];

                        do {
                            size_t bytes_read = fread(in_buf, 1, BUFFER_SIZE, pdf);
                            if (bytes_read == 0) break;

                            zlib_stream.next_in = in_buf;
                            zlib_stream.avail_in = bytes_read;

                            do {
                                zlib_stream.next_out = out_buf;
                                zlib_stream.avail_out = BUFFER_SIZE;
                                inflate(&zlib_stream, Z_NO_FLUSH);
                                
                                size_t out_bytes = BUFFER_SIZE - zlib_stream.avail_out;
                                strncat(page_txt, (char*)out_buf, out_bytes);
                            } while (zlib_stream.avail_out == 0);
                        } while (!strstr((char*)in_buf, "endstream"));

                        inflateEnd(&zlib_stream);
                        found = true;
                        break;
                    }
                }
                break;
            }
        }
    }
    fseek(pdf, pos, SEEK_SET);
    return found;
}
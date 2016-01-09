//
//  main.c
//  Std420ToNv12
//
//  Created by Hank Lee on 10/4/15.
//  Copyright © 2015 Hank Lee. All rights reserved.
//

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>

#include "420ToNv12.h"

#define MAX_WIDTH   3840
#define MAX_HEIGHT  2160

static uint8_t img[MAX_WIDTH * MAX_HEIGHT * 3 / 2];
static uint8_t u_et_v[MAX_WIDTH * MAX_HEIGHT / 2];


bool fill_img_buffer(uint8_t *img, uint32_t desired_bytes)
{
    bool res;
    ssize_t rd_byte;
    uint8_t *pu8 = img;
    
    res = true;
    
    while (desired_bytes > 0)
    {
        rd_byte = read(STDIN_FILENO, pu8, desired_bytes);
        
        if (rd_byte == 0) // EOF
        {
            return false;
        }
        
        pu8 += rd_byte;
        desired_bytes -= rd_byte;
    }
    
    return res;
}


int main(int argc, const char * argv[]) {
    uint8_t *y;
    uint8_t *u;
    uint8_t *v;
    
    uint32_t width;
    uint32_t height;
    uint32_t wxh;

    //uint8_t *u_et_v;
    
    if (argc < 3)
    {
        fprintf(stderr, "useage: %s [width] [height]\n", argv[0]);
        
        return -1;
    }
    
    width   = 0;
    height  = 0;
    wxh     = 0;
    
    width   = atoi(argv[1]);
    height  = atoi(argv[2]);
    
    wxh = width * height;
    
    y = img;
    u = y + wxh;
    v = u + wxh / 4;
    
    fprintf(stderr, "Processing: ");
    
    while (1)
    {
        if (fill_img_buffer(img, wxh * 3 / 2))
        {
            planar_to_interleave
            (
             wxh,
             u_et_v,
             u,
             v
             );
            
            write(STDOUT_FILENO, y, wxh);
            write(STDOUT_FILENO, u_et_v, wxh / 2);
        }
        else
        {
            break;
        }
        fprintf(stderr, ".");
        fflush(stderr);
    }
    
    fprintf(stderr, "Done\n");
    
    return 0;
}


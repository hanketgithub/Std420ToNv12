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


bool fill_buffer(void *buf, uint32_t desired_bytes)
{
    bool res;
    ssize_t rd_byte;
    char *cp = buf;
    
    res = true;
    
    while (desired_bytes > 0)
    {
        rd_byte = read(STDIN_FILENO, cp, desired_bytes);
        
        if (rd_byte == 0) // EOF
        {
            return false;
        }
        
        cp += rd_byte;
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
    
    uint8_t *img;
    uint8_t *u_et_v_dst;
    
    if (argc < 3)
    {
        fprintf(stderr, "useage: %s [width] [height]\n", argv[0]);
        
        return -1;
    }
    
    width       = 0;
    height      = 0;
    wxh         = 0;
    img         = NULL;
    u_et_v_dst  = NULL;
    
    width   = atoi(argv[1]);
    height  = atoi(argv[2]);
    
    wxh = width * height;
    
    img = malloc(wxh * 3 / 2);
    u_et_v_dst = malloc(wxh / 2);
    
    y = img;
    u = y + wxh;
    v = u + wxh / 4;
    
    fprintf(stderr, "Processing: ");
    
    while (1)
    {
        if (fill_buffer(img, wxh * 3 / 2))
        {
            planar_to_interleave
            (
             wxh,
             u_et_v_dst,
             u,
             v
             );
            
            write(STDOUT_FILENO, y, wxh);
            write(STDOUT_FILENO, u_et_v_dst, wxh / 2);
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


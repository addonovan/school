#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <X11/Xlib.h>

#define DIM 512

#define TASK1_VERT  0
#define TASK1_HORZ  0
#define TASK2       1 

#if TASK1_VERT + TASK1_HORZ + TASK2 == 0
    #error "One of TASK1_VERT, TASK1_HORZ, TASK2 must be set to `1`"
#endif

typedef float Pixel;

struct Image {
    Pixel pixels[DIM][DIM];
    int width;
    int height;
};
typedef struct Image Image;

//
// Image Processing Declarations 
//

/**
 * Processes the source `image` and returns an image of the vertical edges in
 * the source.
 */
Image vertical_edge_detection(const Image* image);

/**
 * Processes the source `image` and returns an image of the horizontal edges in
 * the source.
 */
Image horizontal_edge_detection(const Image* image);

/**
 * Processes the `image` and uses the `roi` selection from the image as the
 * template to match against using normalized corss correlation.
 */
Image selected_template_matching(const Image* image);

//
// Utility Function Declarations
//

Image make_image(const unsigned char input[DIM][DIM], const int size[2]);

void copy_image(unsigned char out[DIM][DIM], int size[2], const Image* src);

Image crop_image(const Image* input, int x, int y, int width, int height);

Pixel normalized_cross_correlation(const Image* source, const Image* template);

Image normalize_pixel_values(const Image* src);

Image convolve(const Image* input, const Image* template);


/******************************************************************/
/* This structure contains the coordinates of a box drawn with    */
/* the left mouse button on the image window.                     */
/* roi.x , roi.y  - left upper corner's coordinates               */
/* roi.width , roi.height - width and height of the box           */
/******************************************************************/
extern XRectangle roi;

/******************************************************************/
/* Main processing routine. This is called upon pressing the      */
/* Process button of the interface.                               */
/* image  - the original greyscale image                          */
/* size   - the actual size of the image                          */
/* proc_image - the image representation resulting from the       */
/*              processing. This will be displayed upon return    */
/*              from this function.                               */
/******************************************************************/
void process_image(image, size, proc_img)
unsigned char image[DIM][DIM];
int size[2];
unsigned char proc_img[DIM][DIM];
{
    Image input = make_image(image, size);

#if TASK1_VERT == 1
    Image output = vertical_edge_detection(&input);
#elif TASK1_HORZ == 1
    Image output = horizontal_edge_detection(&input);
#elif TASK2 == 1
    Image output = selected_template_matching(&input);
#else
#error "No task was set!"
#endif

    copy_image(proc_img, size, &output);
}

//
// Task Implementation
// 

Image vertical_edge_detection(const Image* input) {
    Image template;
    template.width = 3;
    template.height = 3;

    template.pixels[0][0] = -1;
    template.pixels[0][1] = -1;
    template.pixels[0][2] = -1;

    template.pixels[1][0] = 0;
    template.pixels[1][1] = 0;
    template.pixels[1][2] = 0;

    template.pixels[2][0] = 1;
    template.pixels[2][1] = 1;
    template.pixels[2][2] = 1;

    Image convolved = convolve(input, &template);
    return normalize_pixel_values(&convolved);
}

Image horizontal_edge_detection(const Image* input) {
    Image template;
    template.width = 3;
    template.height = 3;

    template.pixels[0][0] = -1;
    template.pixels[0][1] =  0;
    template.pixels[0][2] =  1;

    template.pixels[1][0] = -1;
    template.pixels[1][1] =  0;
    template.pixels[1][2] =  1;

    template.pixels[2][0] = -1;
    template.pixels[2][1] =  0;
    template.pixels[2][2] =  1;

    Image convolved = convolve(input, &template);
    return normalize_pixel_values(&convolved);
}

Image selected_template_matching(const Image* image) {
    // extract the template
    Image template = crop_image(image, roi.x, roi.y, roi.width, roi.height);
    Image convolved = convolve(image, &template);
    return normalize_pixel_values(&convolved);
}

//
// Utility Implementations
// 

Image make_image(const unsigned char input[DIM][DIM], const int size[2]) {
    Image out;
    memset(out.pixels, 0, sizeof(out.pixels));
    out.width = size[0];
    out.height = size[1];

    for (int i = 0; i < size[0]; i++) {
        for (int j = 0; j < size[1]; j++) {
            out.pixels[i][j] = input[i][j];
        }
    }

    return out;
}

void copy_image(unsigned char out[DIM][DIM], int size[2], const Image* src) {
    size[0] = src->width;
    size[1] = src->height;
    memset(out, 0, DIM * DIM * sizeof(unsigned char));

    for (int i = 0; i < size[0]; i++) {
        for (int j = 0; j < size[1]; j++) {
            int pixel = src->pixels[i][j];
            pixel = pixel > 255 ? 255 : pixel;
            pixel = pixel < 0 ? 0 : pixel;
            out[i][j] = pixel;
        }
    }
}

Image crop_image(const Image* input, int x, int y, int width, int height) {
    Image output;
    output.width = width;
    output.height = height;

    for (int i = x; i < x + width; i++) {
        for (int j = y; j < y + height; j++) {
            output.pixels[i - x][j - y] = input->pixels[i][j];
        }
    }

    return output;
}

Pixel normalized_cross_correlation(const Image* source, const Image* template) {
    // calculate the averages
    int size = template->width * template->height;
    Pixel avg_source = 0;
    Pixel avg_template = 0;
    for (int i = 0; i < template->width; i++) {
        for (int j = 0; j < template->height; j++) {
            avg_source += source->pixels[i][j];
            avg_template += template->pixels[i][j];
        }
    }
    avg_source /= size;
    avg_template /= size;

    // calculate the standard deviations
    Pixel stddev_source = 0;
    Pixel stddev_template = 0;
    for (int i = 0; i < template->width; i++) {
        for (int j = 0; j < template->height; j++) {
            stddev_source = source->pixels[i][j] - avg_source; 
            stddev_template = template->pixels[i][j] - avg_template;
        }
    }
    stddev_source = sqrt(stddev_source);
    stddev_template = sqrt(stddev_template);

    // finally, actually calculate the pixel value
    Pixel accumulator = 0;
    for (int i = 0; i < template->width; i++) {
        for (int j = 0; j < template->height; j++) {
            accumulator += (source->pixels[i][j] - avg_source)
                        * (template->pixels[i][j] - avg_template); 
        }
    }
    accumulator /= (stddev_source * stddev_template);

    return accumulator;
}

Image normalize_pixel_values(const Image* src) {
    Image output;
    output.width = src->width;
    output.height = src->height;

    Pixel max = -INT_MAX;
    Pixel min = INT_MAX;
    for (int i = 0; i < src->width; i++) {
        for (int j = 0; j < src->height; j++) {
            Pixel px = src->pixels[i][j];
            max = px > max ? px : max;
            min = px < min ? px : min;
        }
    }

    for (int i = 0; i < src->width; i++) {
        for (int j = 0; j < src->height; j++) {
            Pixel px = src->pixels[i][j];
            output.pixels[i][j] = ((px - min) * (255 - 0)) / (max - min);
        }
    }
    
    max = -INT_MAX;
    min = INT_MAX;
    for (int i = 0; i < output.width; i++) {
        for (int j = 0; j < output.height; j++) {
            Pixel px = output.pixels[i][j];
            max = px > max ? px : max;
            min = px < min ? px : min;
        }
    }

    return output;
}

Image convolve(const Image* input, const Image* template) {
    int col_offset = template->width / 2;
    int row_offset = template->height / 2;

    int template_size = template->width * template->height;

    // trim the output image's dimensions to compensate for the lost border pixels
    // keep in mind that with integer divison (x / 2 * 2 != x) if x is odd
    Image output;
    memset(output.pixels, 0, sizeof(output.pixels));
    output.width = input->width - (col_offset * 2);
    output.height = input->height - (row_offset * 2);

    // for each pixel in the input, which corresponds to a pixel in the output
    for (int i = col_offset; i < input->width - col_offset; i++) {
        for (int j = row_offset; j < input->height - row_offset; j++) {

#if TASK1_VERT == 1 || TASK1_HORZ == 1
            // apply the convolution
            double accumulator = 0;
            for (int idiff = -col_offset; idiff <= col_offset; idiff++) {
                for (int jdiff = -row_offset; jdiff <= row_offset; jdiff++) {
                    int template_x = col_offset + idiff;
                    int template_y = row_offset + jdiff;
                    int weight = template->pixels[template_x][template_y];

                    int input_x = i + idiff;
                    int input_y = j + jdiff;
                    int pixel = input->pixels[input_x][input_y];
            
                    accumulator += (weight * pixel);
                }
            }
            accumulator /= template_size;
#elif TASK2 == 1
            // apply normalized cross correlation 
            Image cropped_source = crop_image(input, i, j, template->width, template->height);
            double accumulator = normalized_cross_correlation(&cropped_source, template);
#endif
            
            // keep the accumulator within [0, 255]
            accumulator = accumulator < 0 ? 0 : accumulator;
            accumulator = accumulator > 255 ? 255 : accumulator;

            // calculate the pixel location in the offset
            int out_i = i - col_offset;
            int out_j = j - row_offset;

            // put the accumulator value into the output image
            output.pixels[out_i][out_j] = accumulator;
        }
    }


    return output;
}


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

int NUM_THREADS = 10;

double c_x_min;
double c_x_max;
double c_y_min;
double c_y_max;

double pixel_width;
double pixel_height;

int iteration_max = 200;

int image_size;
unsigned char **image_buffer;

int i_x_max;
int i_y_max;
int image_buffer_size;

int gradient_size = 16;
int colors[17][3] = {
                        {66, 30, 15},
                        {25, 7, 26},
                        {9, 1, 47},
                        {4, 4, 73},
                        {0, 7, 100},
                        {12, 44, 138},
                        {24, 82, 177},
                        {57, 125, 209},
                        {134, 181, 229},
                        {211, 236, 248},
                        {241, 233, 191},
                        {248, 201, 95},
                        {255, 170, 0},
                        {204, 128, 0},
                        {153, 87, 0},
                        {106, 52, 3},
                        {16, 16, 16},
                    };

struct compute_mandelbrot_args {
    int i_x_start;
    int i_y_start;
    int i_x_end;
    int i_y_end;
};

void allocate_image_buffer(){
    int rgb_size = 3;
    image_buffer = (unsigned char **) malloc(sizeof(unsigned char *) * image_buffer_size);

    for(int i = 0; i < image_buffer_size; i++){
        image_buffer[i] = (unsigned char *) malloc(sizeof(unsigned char) * rgb_size);
    };
};

void init(int argc, char *argv[]){
    if(argc < 6){
        printf("usage: ./mandelbrot_pth c_x_min c_x_max c_y_min c_y_max image_size\n");
        printf("examples with image_size = 11500:\n");
        printf("    Full Picture:         ./mandelbrot_pth -2.5 1.5 -2.0 2.0 11500\n");
        printf("    Seahorse Valley:      ./mandelbrot_pth -0.8 -0.7 0.05 0.15 11500\n");
        printf("    Elephant Valley:      ./mandelbrot_pth 0.175 0.375 -0.1 0.1 11500\n");
        printf("    Triple Spiral Valley: ./mandelbrot_pth -0.188 -0.012 0.554 0.754 11500\n");
        exit(0);
    }
    else{
        sscanf(argv[1], "%lf", &c_x_min);
        sscanf(argv[2], "%lf", &c_x_max);
        sscanf(argv[3], "%lf", &c_y_min);
        sscanf(argv[4], "%lf", &c_y_max);
        sscanf(argv[5], "%d", &image_size);

        i_x_max           = image_size;
        i_y_max           = image_size;
        image_buffer_size = image_size * image_size;

        pixel_width       = (c_x_max - c_x_min) / i_x_max;
        pixel_height      = (c_y_max - c_y_min) / i_y_max;
    };
};

void update_rgb_buffer(int iteration, int x, int y){
    int color;

    if(iteration == iteration_max){
        image_buffer[(i_y_max * y) + x][0] = colors[gradient_size][0];
        image_buffer[(i_y_max * y) + x][1] = colors[gradient_size][0];
        image_buffer[(i_y_max * y) + x][2] = colors[gradient_size][0];
    }
    else{
        color = iteration % gradient_size;

        image_buffer[(i_y_max * y) + x][0] = colors[color][0];
        image_buffer[(i_y_max * y) + x][1] = colors[color][1];
        image_buffer[(i_y_max * y) + x][2] = colors[color][2];
    };
};

void write_to_file(){
    FILE * file;
    char * filename               = "output.ppm";
    char * comment                = "# ";

    int max_color_component_value = 255;

    file = fopen(filename,"wb");

    fprintf(file, "P6\n %s\n %d\n %d\n %d\n", comment,
            i_x_max, i_y_max, max_color_component_value);

    for(int i = 0; i < image_buffer_size; i++){
        fwrite(image_buffer[i], 1 , 3, file);
    };

    fclose(file);
};

void compute_mandelbrot(int i_x_start, int i_y_start, int i_x_end, int i_y_end){
    double z_x;
    double z_y;
    double z_x_squared;
    double z_y_squared;
    double escape_radius_squared = 4;

    int iteration;
    int i_x;
    int i_y;

    double c_x;
    double c_y;

    for(i_y = i_y_start; i_y < i_y_end; i_y++){
        c_y = c_y_min + i_y * pixel_height;

        if(fabs(c_y) < pixel_height / 2){
            c_y = 0.0;
        };

        for(i_x = i_x_start; i_x < i_x_end; i_x++){
            c_x         = c_x_min + i_x * pixel_width;

            z_x         = 0.0;
            z_y         = 0.0;

            z_x_squared = 0.0;
            z_y_squared = 0.0;

            for(iteration = 0;
                iteration < iteration_max && \
                ((z_x_squared + z_y_squared) < escape_radius_squared);
                iteration++){
                z_y         = 2 * z_x * z_y + c_y;
                z_x         = z_x_squared - z_y_squared + c_x;

                z_x_squared = z_x * z_x;
                z_y_squared = z_y * z_y;
            };

            update_rgb_buffer(iteration, i_x, i_y);
        };
    };
};

/* Wrapper to compute_mandelbrot - use with pthread_create() */
void *_compute_mandelbrot(void *thread_args) {
    struct compute_mandelbrot_args *args;
    args = (struct compute_mandelbrot_args *) thread_args;
    printf("DEBUG: Thread running for rows %d to %d\n", args->i_y_start, args->i_y_end);

    compute_mandelbrot(args->i_x_start, args->i_y_start, args->i_x_end, args->i_y_end);

    printf("DEBUG: Thread done for rows %d to %d\n", args->i_y_start, args->i_y_end);
    pthread_exit(NULL);
}

void threaded_compute_mandebrot() {
    pthread_t threads[NUM_THREADS];
    struct compute_mandelbrot_args thread_data_array[NUM_THREADS];

    int t, rc;
    int start_row, end_row, rows_per_thread;

    rows_per_thread = image_size / (NUM_THREADS - 1);
    printf("DEBUG: Rows per thread: %d\n", rows_per_thread);

    t = 0;
    start_row = 0;
    end_row = rows_per_thread;

    while (start_row < i_y_max) {

        thread_data_array[t].i_x_start = 0;
        thread_data_array[t].i_y_start = start_row;
        thread_data_array[t].i_x_end   = i_x_max;
        thread_data_array[t].i_y_end   = end_row;

        printf("DEBUG: Creating thread %d for rows %d to %d.\n", t, start_row, end_row);

        rc = pthread_create(&threads[t], NULL, _compute_mandelbrot, (void*) &thread_data_array[t]);
        if (rc) {
            printf("Error creating thread: pthread_create() returned %d\n", rc);
            exit(-1);
        }

        start_row = end_row;
        end_row = start_row + rows_per_thread;
        if (end_row > i_y_max) {
            end_row = i_y_max;
        }

        t++;
    }

    printf("DEBUG: All threads created, will start joining...\n");

    for (t = 0; t < NUM_THREADS; t++) {
        printf("DEBUG: Joining thread %d...\n", t);
        pthread_join(threads[t], NULL);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    init(argc, argv);

    allocate_image_buffer();

    threaded_compute_mandebrot();

    /* write_to_file(); */

    return 0;
};

#define M_PI 3.14159265358979323846
#define M_G  9.81
#define EARTH_RADIUS 6400000
#define A 20
#define MAX_FORM_SIZE 600
#define SCALE_WIDTH 60
#define LAND_UP 1000
#define MAX_STR_LEN 256

int t;
double *delta_t;
int time_moments;
int output_moments=30;
bool running = false;
long int size_x, size_y;      // Sizes of matrix
double start_x, end_x;      // Range X (parallels) (degrees)
double start_y, end_y;      // Range Y (meredians) (degrees)
double delta_x, delta_y;    // Steps on X & Y (degrees)
double delta_x_m, *delta_y_m;// Steps on X & Y (metres)
double izobata;             // Izobata (metres)
double h_max;               // Deeper place (metres)
double **eta_new;
double **bottom;  // Earth relief (source data)
double **h;       // Bottom relief for calculating
double **eta;     // Water surface
double **eta_old; // OLD Water surface
double **visota;
double **u;       // U speed of water
double **v;       // V speed of water
double **u_old;   // OLD U speed of water
double **v_old;   // OLD V speed of water
int terr_cnt;     // Count of bricks
int point_cnt;    // Count of points
int **terr_up;    // Bricks
double **t_h_v_up;
double **terr_points;    // Brick's corners coordinates
double **point_points;   // Point's corners coordinates

double **terr_tmp;    // Brick's corners coordinates
double **point_tmp;   // Point's corners coordinates
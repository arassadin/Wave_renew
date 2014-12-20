#define M_PI 3.14159265358979323846
#define M_G  9.81
#define EARTH_RADIUS 6371000
#define LAND_UP 1000
#define MAX_STR_LEN 256

int currentCalculationTime;
int calculationTime;
int outTime;
bool isProcessing = false;
int mapSizeX, mapSizeY;
double startX_dgr, endX_dgr;
double startY_dgr, endY_dgr;
double isobath;
double maxHeight;
double **terrian;
double **h;
double **eta;
double **eta_old;
double **visota;
double **u;
double **v;
double **u_old;
double **v_old;
int hearthBricksQ;
int watchingPointsQ;
double **terr_points;
double **point_points;

double delta_x, delta_y;
double delta_x_m, *delta_y_m;
double **t_h_v_up;
int **terr_up;
double *delta_t;
double **terr_tmp;
double **point_tmp;
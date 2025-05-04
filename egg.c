#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define screen_width 120
#define screen_height 100
#define pi 3.141592653589793

const float phi_spacing = 0.02;
const float y_spacing = 0.03;
const float K2 = 5;
const float ymin = -2.0; 
const float ymax = 2.0;

const float K1 = screen_width * K2 * 1.5 / (8 * 2.5); 

void render_frame(float A, float B) {
    float cosA = cos(A), sinA = sin(A);
    float cosB = cos(B), sinB = sin(B);

    char output[screen_height][screen_width];
    float zbuffer[screen_height][screen_width];

    memset(output, ' ', sizeof(output));
    memset(zbuffer, 0, sizeof(zbuffer));

    for (float y = ymin; y <= ymax; y += y_spacing) {
        float x_squared = pow(2, y) - y * y;
        if (x_squared <= 0) continue;
        float x = sqrt(x_squared);

        for (int sign = -1; sign <= 1; sign += 2) {
            float x_signed = sign * x;

            for (float phi = 0; phi < 2 * pi; phi += phi_spacing) {
                float cosphi = cos(phi), sinphi = sin(phi);

                float xp = x_signed * cosphi;
                float yp = y;
                float zp = x_signed * sinphi;

                float x_rot = xp * cosB + zp * sinA * sinB - yp * cosA * sinB;
                float y_rot = xp * sinB - zp * sinA * cosB + yp * cosA * cosB;
                float z_rot = K2 + zp * cosA + yp * sinA;

                float ooz = 1 / z_rot;
                int proj_x = (int)(screen_width / 2 + K1 * ooz * x_rot);
                int proj_y = (int)(screen_height / 2 - K1 * ooz * y_rot) + 25;

                float L = cosphi * sinB - cosA * sinphi;
                if (L > 0 && proj_x >= 0 && proj_x < screen_width && proj_y >= 0 && proj_y < screen_height) {
                    if (ooz > zbuffer[proj_y][proj_x]) {
                        zbuffer[proj_y][proj_x] = ooz;
                        int luminance_index = (int)(L * 8);
                        const char *lum_chars = ".,-~:;=!*#$@";
                        if (luminance_index >= 0 && luminance_index < 12)
                            output[proj_y][proj_x] = lum_chars[luminance_index];
                    }
                }
            }
        }
    }

    printf("\x1b[H");
    for (int j = 0; j < screen_height; j++) {
        for (int i = 0; i < screen_width; i++) {
            putchar(output[j][i]);
        }
        putchar('\n');
    }
}

int main() {
    float A = 0, B = 0;
    while (1) {
        render_frame(A, B);
        A += 0.05;
        B += 0.1;  
        usleep(30000); 
    }
    return 0;
}

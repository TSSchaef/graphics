#include "vector3.h"

inline void init(vector3 *v, double e1, double e2, double e3){
    v->e[0] = e1;
    v->e[1] = e2;
    v->e[2] = e3;
}

inline void copy(vector3 *v, const vector3 w){
    v->e[0] = w.e[0];
    v->e[1] = w.e[1];
    v->e[2] = w.e[2];
}

inline void invert(vector3 *v){
   v->e[0] *= -1;
   v->e[1] *= -1;
   v->e[2] *= -1;
}

inline void add_vector(vector3 *v, const vector3 w){
    v->e[0] += w.e[0];
    v->e[1] += w.e[1];
    v->e[2] += w.e[2];
}

inline void scale(vector3 *v, double t){
    v->e[0] *= t;
    v->e[1] *= t;
    v->e[2] *= t;
}

inline double length(const vector3 v){
    return sqrt(v.e[0]*v.e[0] + v.e[1]*v.e[1] + v.e[2]*v.e[2]);
}

inline double length_squared(const vector3 v){
    return v.e[0]*v.e[0] + v.e[1]*v.e[1] + v.e[2]*v.e[2];
}

inline double illuminance(const color v){
    return 0.2126 *v.e[0] + 0.7152 * v.e[1] + 0.0722 * v.e[2];
}

inline bool near_zero(const vector3 v){
    double s = 0.00000001;
    return (v.e[0] < s && v.e[0] > -1 * s) && (v.e[1] < s && v.e[1] > -1 * s) && (v.e[2] < s && v.e[2] > -1 * s);
}

inline double dot(const vector3 v, const vector3 w){
    return v.e[0]*w.e[0] + v.e[1]*w.e[1] + v.e[2]*w.e[2];
}

inline vector3 cross(const vector3 u, const vector3 v){
   vector3 ans;
   init(&ans, (u.e[1] * v.e[2]) - (u.e[2] * v.e[1]), (u.e[2] * v.e[0]) - (u.e[0] * v.e[2]), (u.e[0] * v.e[1]) - (u.e[1] * v.e[0]));

   return ans;
}

inline void unit_vector(vector3 *v){
    double l = length(*v);

    v->e[0] /= l;
    v->e[1] /= l;
    v->e[2] /= l;
}

inline vector3 attenuate(const vector3 v, const vector3 w){
    vector3 ans;
    copy(&ans, w);
    ans.e[0] *= v.e[0];
    ans.e[1] *= v.e[1];
    ans.e[2] *= v.e[2];
    return ans;
}

inline vector3 reflect(const vector3 v, const vector3 n){
    vector3 ans;
    copy(&ans, n);
    scale(&ans, -2 * dot(v, n));
    add_vector(&ans, v);
    return ans;
}

inline vector3 refract(const vector3 v, const vector3 n, double etai_over_etat){
    vector3 invV;
    copy(&invV, v);
    invert(&invV);
    double cos_theta = dot(invV, n); 
    cos_theta = (cos_theta > 1.0) ? 1.0 : cos_theta;

    vector3 r_out_perp;
    copy(&r_out_perp, n);
    scale(&r_out_perp, cos_theta);
    add_vector(&r_out_perp, v);
    scale(&r_out_perp, etai_over_etat);

    double num = 1.0 - length_squared(r_out_perp);
    num = (num < 0) ? -1 * num : num;
    num = -1 * sqrt(num);
    vector3 r_out_parallel;
    copy(&r_out_parallel, n);
    scale(&r_out_parallel, num);

    add_vector(&r_out_perp, r_out_parallel);

    return r_out_perp;
}

inline void print(const vector3 v){
    printf("%lf %lf %lf\n", v.e[0], v.e[1], v.e[2]);
}

static inline uint8_t get_exp(double max_rgb){
    uint8_t exp = 128;
    if(max_rgb >= 256){
        while(max_rgb > 256){
            max_rgb /= 2;
            exp++;
            if(exp >= 255){
                return 255;
            }
        }
    } else {
        while(max_rgb < 256){
            max_rgb *= 2;
            exp--;
            if(exp <= 0){
                return 0;
            }
        }
        // Went 1 two far
        exp++;
    }
    return exp;
}

static inline double linear_to_gamma(double x){
    if(x > 0)
        return pow(x, 1.0/2.2);
    return 0;
}

void print_color(const color c_in, uint8_t pixel[3], bool linear){
    color c;
    copy(&c, c_in);

    //Removing any NaN's 
    if(c.e[r] != c.e[r]) c.e[r] = 0.0;
    if(c.e[b] != c.e[b]) c.e[b] = 0.0;
    if(c.e[g] != c.e[g]) c.e[g] = 0.0;

    if(linear){
        c.e[r] = 255.999 * linear_to_gamma(c.e[r]);
        c.e[g] = 255.999 * linear_to_gamma(c.e[g]);
        c.e[b] = 255.999 * linear_to_gamma(c.e[b]);
    } else {
        c.e[r] = 255.999 * c.e[r];
        c.e[g] = 255.999 * c.e[g];
        c.e[b] = 255.999 * c.e[b];
    }

    //finding exponent
    uint8_t exp;
    if(c.e[r] > c.e[g]){
        if(c.e[r] > c.e[b]){
           exp = get_exp(c.e[r]); 
        } else {
           exp = get_exp(c.e[b]); 
        }
    } else {
        if(c.e[g] > c.e[b]){
           exp = get_exp(c.e[g]); 
        } else {
           exp = get_exp(c.e[b]); 
        }
    }

    pixel[r] = (uint8_t) exp > 0 ? (c.e[r] / pow(2, exp - 128)) : 0;
    pixel[g] = (uint8_t) exp > 0 ? (c.e[g] / pow(2, exp - 128)) : 0;
    pixel[b] = (uint8_t) exp > 0 ? (c.e[b] / pow(2, exp - 128)) : 0;
    //setting exponent
    pixel[3] = exp;
}

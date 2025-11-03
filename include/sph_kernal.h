#ifndef SPH_KERNAL_H
#define SPH_KERNAL_H

const double PId = 3.14159265358979;
// An sph kernal defines a curve with an area of one which represents the influence of 
// a particle on the area around it. 

// This is the basic kernal implementation from Doyub Kim's fluid engine development book
struct SphStdKernal3 {
    // 315 / (64 * pi * h^3) (1 - (r^2 / h^2)^3) when 0 <= r < = h
    double h1, h2, h3;

    SphStdKernal3() : h1(0), h2(0), h3(0) {};
    explicit SphStdKernal3(double radius) : h1(radius), h2(radius * radius), h3(h2 * radius) {};

    double operator()(double distance) const;
};

struct SphStdKernal2 {
    double h, h2;

    SphStdKernal2() : h(0), h2(0) {};
    explicit SphStdKernal2(double radius) : h(radius), h2(radius * radius) {};

    double operator()(double distance) const;
};

struct SphSpikyKernal3 {
    double h1, h2, h3;

    SphSpikyKernal3() : h1(0), h2(0), h3(0) {};
    explicit SphSpikyKernal3(double radius) : h1(radius), h2 (h1 * h1), h3(h1 * h2) {};

    double operator()(double distance) const;
};

struct SphSpikyKernal2 {
    double h1, h2;

    SphSpikyKernal2() : h1(0), h2(0) {};
    explicit SphSpikyKernal2(double radius) : h1(radius), h2(radius * radius) {};

    double operator()(double distance) const;
};


inline double SphStdKernal3::operator()(double distance) const{
    if (distance * distance >= h2){
        return 0.0;
    } else {
        double x = (1.0 - (distance * distance) / h2);
        return 315.0 / (64.0 * PId * h3) * x * x * x;
    }
}

inline double SphStdKernal2::operator()(double distance) const {
    if (distance * distance >= h2){
        return 0.0;
    } else {
        double x = (1 - distance * distance / h2);
        return (4.0 / (PId * h2)) * x * x * x;

    }
}

inline double SphSpikyKernal3::operator()(double distance) const{
    if (distance * distance >= h2){
        return 0.0;
    } else {
        double x = (1.0 - (distance) / h1);
        return 15.0 / (PId * h3) * x * x * x;
    }
}

inline double SphSpikyKernal2::operator()(double distance) const{
    if (distance * distance >= h2){
        return 0.0;
    } else {
        double x = (1.0 - (distance) / h1);
        return 10.0 / (PId * h2) * x * x * x;
    }
}

// Spiky implementations are better!


#endif
#ifndef SPH_KERNAL_H
#define SPH_KERNAL_H

#include "vector.h"

const double PId = 3.14159265358979;
// An sph kernal defines a curve with an area of one which represents the influence of 
// a particle on the area around it. 

// This is the basic kernal implementation from Doyub Kim's fluid engine development book
struct SphStdKernal3 {
    // 315 / (64 * pi * h^3) (1 - (r^2 / h^2)^3) when 0 <= r < = h
    double h1, h2, h3, h5;

    SphStdKernal3() : h1(0), h2(0), h3(0), h5(0) {};
    explicit SphStdKernal3(double radius) : h1(radius), h2(radius * radius), h3(h2 * radius), h5(h2 * h3) {};

    double operator()(double distance) const;

    double first_derivative(double distance) const;
    double second_derivative(double distance) const;

    cato::Vec3d gradient(double distance, const cato::Vec3T<double>& direction) const;

};

struct SphStdKernal2 {
    double h, h2, h3, h4;

    SphStdKernal2() : h(0), h2(0), h3(0), h4(0) {};
    explicit SphStdKernal2(double radius) : h(radius), h2(radius * radius), h3(h2 * radius), h4(h2 * h2) {};

    double operator()(double distance) const;

    double first_derivative(double distance) const;
    double second_derivative(double distance) const;
    cato::Vec2d gradient(double distance, const cato::Vec2T<double>& direction) const;
};

struct SphSpikyKernal3 {
    double h1, h2, h3, h4, h5;

    SphSpikyKernal3() : h1(0), h2(0), h3(0), h4(0), h5(0) {};
    explicit SphSpikyKernal3(double radius) : h1(radius), h2 (h1 * h1), h3(h1 * h2), h4(h2 * h2), h5(h3 * h2) {};

    double operator()(double distance) const;
    double first_derivative(double distance) const;
    double second_derivative(double distance) const;
    cato::Vec3d gradient(double distance, const cato::Vec3T<double>& direction) const;
};

struct SphSpikyKernal2 {
    double h1, h2, h3, h4;

    SphSpikyKernal2() : h1(0), h2(0), h3(0), h4(0) {};
    explicit SphSpikyKernal2(double radius) : h1(radius), h2(radius * radius), h3(radius * h2), h4(h2 * h2) {};

    double operator()(double distance) const;
    double first_derivative(double distance) const;
    double second_derivative(double distance) const;
    cato::Vec2d gradient(double distance, const cato::Vec2T<double>& direction) const;
};


inline double SphStdKernal3::operator()(double distance) const{
    if (distance * distance >= h2){
        return 0.0;
    } else {
        double x = (1.0 - (distance * distance) / h2);
        return 315.0 / (64.0 * PId * h3) * x * x * x;
    }
}

inline double SphStdKernal3::first_derivative(double distance) const {
    if (distance * distance >= h2){
        return 0.0;
    } else {
        double x = (1 - distance * distance / h2);
        return -945.0 / (32.0 * PId * h5) * distance * x * x;
    }
}

inline double SphStdKernal3::second_derivative(double distance) const {
    if (distance * distance >= h2){
        return 0.0;
    } else {
        double x = distance * distance / h2;
        return 945.0 / (32.0 * PId * h5) * (3 * x - 1) * (1 - x);
    }
}

inline cato::Vec3d SphStdKernal3::gradient(double distance, const cato::Vec3T<double>& direction) const {
    // Direction is assumed to be normalized
    return -first_derivative(distance) * direction;
}

inline double SphStdKernal2::operator()(double distance) const {
    if (distance >= h){
        return 0.0;
    } else {
        double x = (1 - distance * distance / h2);
        return (4.0 / (PId * h2)) * x * x * x;

    }
}

inline double SphStdKernal2::first_derivative(double distance) const {
    if (distance >= h){
        return 0.0;
    } else {
        double x = (1 - distance * distance / h2);
        return -24.0 / (PId * h2 * h2) * distance * x * x;
    }
}

inline double SphStdKernal2::second_derivative(double distance) const {
    if (distance >= h){
        return 0.0;
    } else {
        double x = distance * distance / h2;
        return 24.0 / (PId * h4) * (1 - x) * (5 * x - 1);
    }
}

inline cato::Vec2d SphStdKernal2::gradient(double distance, const cato::Vec2T<double>& direction) const {
    // Direction is assumed to be normalized
    return -first_derivative(distance) * direction;
}

inline double SphSpikyKernal3::operator()(double distance) const{
    if (distance >= h1){
        return 0.0;
    } else {
        double x = (1.0 - (distance) / h1);
        return 15.0 / (PId * h3) * x * x * x;
    }
}

inline double SphSpikyKernal3::first_derivative(double distance) const {
    if (distance >= h1){
        return 0.0;
    } else {
        double x = (1.0 - (distance) / h1);
        return -45.0 / (PId * h4) * x * x;
    }
}

inline double SphSpikyKernal3::second_derivative(double distance) const {
    if (distance >= h1){
        return 0.0;
    } else {
        double x = (1.0 - (distance) / h1);
        return 90.0 / (PId * h5) * x;
    }
}

inline cato::Vec3d SphSpikyKernal3::gradient(double distance, const cato::Vec3T<double>& direction) const {
    // Direction is assumed to be normalized
    return -first_derivative(distance) * direction;
}

inline double SphSpikyKernal2::operator()(double distance) const{
    if (distance * distance >= h2){
        return 0.0;
    } else {
        double x = (1.0 - (distance) / h1);
        return 10.0 / (PId * h2) * x * x * x;
    }
}

inline double SphSpikyKernal2::first_derivative(double distance) const {
    if (distance >= h1){
        return 0.0;
    } else {
        double x = (1.0 - (distance) / h1);
        return -30.0 / (PId * h3) * x * x;
    }
}

inline double SphSpikyKernal2::second_derivative(double distance) const {
    if (distance >= h1){
        return 0.0;
    } else {
        double x = (1.0 - (distance) / h1);
        return 60.0 / (PId * h4) * x;
    }
}

inline cato::Vec2d SphSpikyKernal2::gradient(double distance, const cato::Vec2T<double>& direction) const {
    // Direction is assumed to be normalized
    return -first_derivative(distance) * direction;
}


#endif
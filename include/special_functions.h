/*!
 * \file
 * \brief   Special mathematical function header
 * \authors A.Tykvinskiy
 * \date    28.12.2021
 * \version 1.0
 *
 * The header declares specialized mathematical functions and classes that
 * utilized for digital signal processing (DSP).
*/

#ifndef SPECIAL_FUNCTIONS_H
#define SPECIAL_FUNCTIONS_H

// standart headers
#ifndef __ALG_PLATFORM
#include "cmath"
#include "malloc.h"
#endif

/*! \brief converts radians to degrees */
#ifndef __TO_DEGREES
#define __TO_DEGREES(x) ( (x) * 57.295779513082320876798154814105)
#endif

/*! \brief converts degrees to radians */
#ifndef __TO_RADIANS
#define __TO_RADIANS(x) ( (x) * 0.01745329251994329576923690768489)
#endif

/*! \brief sets maximum number of iteration for elliptic functions computation */
#ifndef __ITERATIONS_NUMBER__
#define __ITERATIONS_NUMBER__ 30
#endif

/*! \brief defines 16-bit integer type */
#ifndef __ix16
#define __ix16 short
#endif

/*! \brief defines 32-bit integer type */
#ifndef __ix32
#define __ix32 int
#endif

/*! \brief defines 64-bit integer type */
#ifndef __ix64
#define __ix64 long long
#endif

/*! \brief defines unsigned 16-bit integer type */
#ifndef __uix16
#define __uix16 unsigned short
#endif

/*! \brief defines unsigned 32-bit integer type */
#ifndef __uix32
#define __uix32 unsigned int
#endif

/*! \brief defines unsigned 64-bit integer type */
#ifndef __uix64
#define __uix64 unsigned long long
#endif

/*! \brief defines 32-bit floating point type */
#ifndef __fx32
#define __fx32 float
#endif

/*! \brief defines 64-bit floating point type */
#ifndef __fx64
#define __fx64 double
#endif

/*! \brief defines extended 64-bit floating point type */
#ifndef __fxx64
#define __fxx64 long double
#endif

/*! \brief defines 32-bit floating point type upper limit */
#ifndef __max_fx32
#define __max_fx32 3.402823466e+38
#endif

/*! \brief defines 64-bit floating point type upper limit */
#ifndef __max_fx64
#define __max_fx64 1.7976931348623158e+308
#endif

/*! \brief defines 16-bit integer type upper limit */
#ifndef __max_ix16
#define __max_ix16 32767
#endif

/*! \brief defines 32-bit integer type upper limit */
#ifndef __max_ix32
#define __max_ix32 2147483647
#endif

/*! \brief defines 64-bit integer type upper limit */
#ifndef __max_ix64
#define __max_ix64 9223372036854775807
#endif

/*! \brief defines unsigned 16-bit integer type upper limit */
#ifndef __max_uix16
#define __max_uix16 65535
#endif

/*! \brief defines unsigned 32-bit integer type upper limit */
#ifndef __max_uix32
#define __max_uix32 4294967295
#endif

/*! \brief defines unsigned 64-bit integer type upper limit */
#ifndef __max_uix64
#define __max_uix64 18446744073709551615
#endif

/*! \brief defines 32-bit floating point type lower limit */
#ifndef __min_fx32
#define __min_fx32 1.175494351e-38
#endif

/*! \brief defines 64-bit floating point type lower limit */
#ifndef __min_fx64
#define __min_fx64 2.22507e-308
#endif

/*! \brief defines 16-bit integer type lower limit */
#ifndef __min_ix16
#define __min_ix16 -32767
#endif

/*! \brief defines 32-bit integer type lower limit */
#ifndef __min_ix32
#define __min_ix32 -2147483647
#endif

/*! \brief defines 64-bit integer type lower limit */
#ifndef __min_ix64
#define __min_ix64 -9223372036854775807
#endif

/*! \brief defines unsigned 16-bit integer type lower limit */
#ifndef __min_uix16
#define __min_uix16 0
#endif

/*! \brief defines unsigned 32-bit integer type lower limit */
#ifndef __min_uix32
#define __min_uix32 0
#endif

/*! \brief defines unsigned 64-bit integer type lower limit */
#ifndef __min_uix64
#define __min_uix64 0
#endif

/*! \brief defines 32-bit floating point computer epsilon */
#ifndef __EPSILON_x32__
#define __EPSILON_x32__ 1.19209e-007
#endif

/*! \brief defines 64-bit floating point computer epsilon */
#ifndef __EPSILON_x64__
#define __EPSILON_x64__ 2.22045e-016
#endif

/*! \brief defines extended 64-bit floating point computer epsilon */
#ifndef __EPSILON_xx64__
#define __EPSILON_xx64__ 1.0842e-019
#endif

/*! \brief defines pi */
#ifndef PI0
#define PI0 3.1415926535897932384626433832795
#endif

/*! \brief defines 2*pi */
#ifndef PI2
#define PI2 6.283185307179586476925286766559
#endif

/*! \brief defines pi / 2 */
#ifndef PI_2
#define PI_2 1.5707963267948966192313216916398
#endif

/*! \brief defines pi / 4 */
#ifndef PI_4
#define PI_4 0.78539816339744830961566084581988
#endif

/*! \defgroup <Elliptic_fcn> ( Elliptic functons )
 *  \brief the module collaborates all the elliptic functions and integrals
    @{
*/

/*!
    Jacobi amplitude function
    \param[u] incomplete elliptic integral of the first kind
    \param[k] elliptical modulus
    \return  returns Jacobi amplitude function

    The computes the Jacobi amplitude function using the method of the arithmetic-geometric mean.
*/
extern __fx64 __am__( __fx64 u , __fx64 k )
{
    __fxx64 a[__ITERATIONS_NUMBER__ + 1];
    __fxx64 g[__ITERATIONS_NUMBER__ + 1];
    __fxx64 c[__ITERATIONS_NUMBER__ + 1];
    __fxx64 two_n;
    __fxx64 phi;
    __ix32  n;

    k = fabsl((__fxx64)k);

    // if k is out of range [-1 ; +1] we should set an edge value...
    if     ( k < -1 ) k = -1;
    else if( k > +1 ) k = +1;

    // special cases check: k = 1 and x = 0
    if (k == 0.0) return u;
    if (k == 1.0) return 2.0 * atan(exp(u)) - PI2;

    // arithmetico-geometric mean method variables initialization:
    a[0] = 1.0L;
    g[0] = sqrtl(1.0L - k * k);
    c[0] = k;

    /*
       Gauss arithmetic operations are continued untill the sequence converges to
       the answer that satisfies machinery epsilon accuracy
    */

    two_n = 1.0L;

    for (n = 0; n < __ITERATIONS_NUMBER__; n++)
    {
        if (fabsl(a[n] - g[n]) < (a[n] * __EPSILON_xx64__)) break;
        two_n += two_n;
        a[n + 1] = 0.5L * (a[n] + g[n]);
        g[n + 1] = sqrtl(a[n] * g[n]);
        c[n + 1] = 0.5L * (a[n] - g[n]);
    }

    // backward substitution implementation:
    phi = two_n * a[n] * u;
    for (; n > 0; n--) phi = 0.5L * (phi + asinl(c[n] * sinl(phi) / a[n]));
    return ( __fx64 )phi;
}

/*!
    Elliptic Jacobi SN function
    \param[u] incomplete elliptic integral of the first kind
    \param[k] elliptical modulus
    \return  returns Ellptic Jacobi SN( u , x ) = sin( am( u , x ) ) function
*/
extern __fx64 __sn__( __fx64 u , __fx64 x ) { return sin( __am__( u , x ) ); }

/*!
    Elliptic Jacobi CN function
    \param[u] incomplete elliptic integral of the first kind
    \param[k] elliptical modulus
    \return  returns Ellptic Jacobi CN( u , x ) = cos( am( u , x ) ) function
*/
extern __fx64 __cn__( __fx64 u , __fx64 x ) { return cos( __am__( u , x ) ); }

/*!
    Elliptic Jacobi DN function
    \param[u] incomplete elliptic integral of the first kind
    \param[k] elliptical modulus
    \return  returns Ellptic Jacobi DN( u , x ) = sqrt( 1 - x * x * SN( u , x ) * SN( u , x ) ) function
*/
extern __fx64 __dn__( __fx64 u , __fx64 x ) { __fx64 SN = __sn__( u , x ); return sqrt(1.0 - x * x * SN * SN); }

/*!
    Elliptic Jacobi CD function
    \param[u] incomplete elliptic integral of the first kind
    \param[k] elliptical modulus
    \return  returns Ellptic Jacobi CD( u , x ) = CN( u , x ) / DN( u , x ) function
*/
extern __fx64 __cd__( __fx64 u , __fx64 x ) { return __cn__( u , x ) / __dn__( u , x ); }

/*!
    Elliptic Jacobi SD function
    \param[u] incomplete elliptic integral of the first kind
    \param[k] elliptical modulus
    \return  returns Ellptic Jacobi SD( u , x ) = SN( u , x ) / DN( u , x ) function
*/
extern __fx64 __sd__( __fx64 u , __fx64 x ) { return __sn__( u , x ) / __dn__( u , x ); }

/*!
    Elliptic Jacobi ND function
    \param[u] incomplete elliptic integral of the first kind
    \param[k] elliptical modulus
    \return  returns Ellptic Jacobi ND( u , x ) = SN( u , x ) / DN( u , x ) function
*/
extern __fx64 __nd__( __fx64 u , __fx64 x ) { return 1 / __dn__( u , x ); }

/*!
    Elliptic Jacobi DC function
    \param[u] incomplete elliptic integral of the first kind
    \param[k] elliptical modulus
    \return  returns Ellptic Jacobi DC( u , x ) = 1 / ND( u , x ) function
*/
extern __fx64 __dc__( __fx64 u , __fx64 x ) { return __dn__( u , x ) / __cn__( u , x ); }

/*!
    Elliptic Jacobi NC function
    \param[u] incomplete elliptic integral of the first kind
    \param[k] elliptical modulus
    \return  returns Ellptic Jacobi NC( u , x ) = 1 / CN( u , x ) function
*/
extern __fx64 __nc__( __fx64 u , __fx64 x ) { return 1 / __cn__(u, x); }

/*!
    Elliptic Jacobi SC function
    \param[u] incomplete elliptic integral of the first kind
    \param[k] elliptical modulus
    \return  returns Ellptic Jacobi SC( u , x ) = SN( u , x ) / CN( u , x ) function
*/
extern __fx64 __sc__( __fx64 u , __fx64 x ) { return __sn__( u , x ) / __cn__( u , x ); }

/*!
    Elliptic Jacobi NS function
    \param[u] incomplete elliptic integral of the first kind
    \param[k] elliptical modulus
    \return  returns Ellptic Jacobi NS( u , x ) = 1 / SN( u , x ) function
*/
extern __fx64 __ns__( __fx64 u , __fx64 x ) { return 1 / __sn__( u , x ); }

/*!
    Elliptic Jacobi DS function
    \param[u] incomplete elliptic integral of the first kind
    \param[k] elliptical modulus
    \return  returns Ellptic Jacobi DS( u , x ) = DN( u , x ) / SN( u , x ) function
*/
extern __fx64 __ds__( __fx64 u , __fx64 x ) { return __dn__( u , x ) / __sn__( u , x ); }

/*!
 * Elliptic Jacobi CS function
 * \param[u] incomplete elliptic integral of the first kind
 * \param[k] elliptical modulus
 * \return  returns Ellptic Jacobi CS( u , x ) = CN( u , x ) / SN( u , x ) function
*/
extern __fx64 __cs__( __fx64 u , __fx64 x ) { return __cn__( u , x ) / __sn__( u , x ); }

/*!
 * Elliptic Jacobi inverse SN function
 * \param[sn] Elliptic Jacobi SN function
 * \param[k] elliptical modulus
 * \return  returns Ellptic Jacobi inverse SN function
*/
extern __fx64 __isn__( __fx64 sn , __fx64 x )
{
    // auxiliary variables and arrays:
    __fx64 a[__ITERATIONS_NUMBER__ + 1] ,  g[__ITERATIONS_NUMBER__ + 1] , s[__ITERATIONS_NUMBER__ + 1];
    __fx64 two_n , phi , phi_old , k;
    __ix32 n = 0;

    // initialization:
    k = fabs(x);
    a[0] = 1.0L;
    g[0] = sqrt(1.0 - k * k);

    // forward substitution:
    two_n = 1.0L;
    for (n = 0; n < __ITERATIONS_NUMBER__ - 1; n++)
    {
        s[n]     = 0;
        two_n   += two_n;
        a[n + 1] = 0.5 * (a[n] + g[n]);
        g[n + 1] = sqrt(a[n] * g[n]);
    }

    // backward substitution:
    phi = asin(sn);
    for (n = 1 ; n < __ITERATIONS_NUMBER__ ; n++)
    {

        phi_old = phi;
        phi     = atan( g[n-1]/a[n-1] * tan(phi)) + phi;

        if ( fabs(2*phi_old - phi) < 0.001 )
        {
            s[n] = 2 * s[n - 1];
        }
        else if (fabs(2 * phi_old - phi) > PI_2)
        {
            s[n] = 2 * s[n - 1] + 1;
        }

    }

    return  ( sn < 0 ) ? ( -(phi + s[n - 1] * PI0) / two_n / a[n - 1] ) : ( +(phi + s[n - 1] * PI0) / two_n / a[n - 1] );
}

/*!
 * Elliptic Jacobi inverse CN function
 * \param[cn] elliptic Jacobi CN function
 * \param[x] elliptical modulus
 * \return  returns Ellptic Jacobi inverse CN function
*/
extern __fx64 __icn__( __fx64 sn , __fx64 x )
{
    __fx64 a[ __ITERATIONS_NUMBER__ + 1 ] , g[ __ITERATIONS_NUMBER__ + 1 ] , s[ __ITERATIONS_NUMBER__ + 1 ];
    __fx64 two_n , phi , phi_old , k;
    __ix32 n = 0;

    // initialization:
    k    = fabs(x);
    a[0] = 1.0L;
    g[0] = sqrt(1.0 - k * k);

    // forward substitution:
    two_n = 1.0L;
    for (n = 0; n < __ITERATIONS_NUMBER__ - 1; n++)
    {
        s[n]     = 0;
        two_n   += two_n;
        a[n + 1] = 0.5 * (a[n] + g[n]);
        g[n + 1] = sqrt(a[n] * g[n]);
    }

    // backward substitution:
    phi = acos(sn);
    for (n = 1; n < __ITERATIONS_NUMBER__ ; n++)
    {
        phi_old = phi;
        phi = atan(g[n - 1] / a[n - 1] * tan(phi)) + phi;
        if (fabs(2 * phi_old - phi) < 5 * PI0 / 180 )
        {
            s[n] = 2 * s[n - 1];
        }
        else if (fabs(2 * phi_old - phi) > PI_2)
        {
            s[n] = 2 * s[n - 1] + 1;
        }
    }

    return  +(phi + s[n-1] * PI0) / two_n / a[n-1];
}

/*!
 * Elliptic Jacobi inverse DN function\
 * \param[dn] Elliptic Jacobi DN function
 * \param[k] elliptical modulus
 * \return  returns Ellptic Jacobi inverse DN function
*/
extern __fx64 __idn__( __fx64 dn , __fx64 x )
{
    __fx64 a[ __ITERATIONS_NUMBER__ + 1 ] , g[ __ITERATIONS_NUMBER__ + 1 ] , s[ __ITERATIONS_NUMBER__ + 1 ];
    __fx64 two_n , phi , phi_old , k;
    __ix32 n = 0;

    // initialization:
    k = fabs(x);
    a[0] = 1.0L;
    g[0] = sqrt(1.0 - k * k);

    // forward substitution:
    two_n = 1.0L;
    for (n = 0; n < __ITERATIONS_NUMBER__ - 1; n++)
    {
        s[n] = 0;
        two_n += two_n;
        a[n + 1] = 0.5 * (a[n] + g[n]);
        g[n + 1] = sqrt(a[n] * g[n]);
    }

    // backward substitution:
    phi = asin( sqrt( (1  - dn * dn) / k / k ) );
    for (n = 1; n < __ITERATIONS_NUMBER__; n++)
    {
        phi_old = phi;
        phi = atan(g[n - 1] / a[n - 1] * tan(phi)) + phi;

        if (fabs(2 * phi_old - phi) < 5 * PI0 / 180)
        {
            s[n] = 2 * s[n - 1];
        }
        else if (fabs(2 * phi_old - phi) > PI_2)
        {
            s[n] = 2 * s[n - 1] + 1;
        }
    }

    return  +(phi + s[n - 1] * PI0) / two_n / a[n - 1];
}

/*!
 * Elliptic Jacobi inverse CD function\
 * \param[dn] Elliptic Jacobi CD function
 * \param[k] elliptical modulus
 * \return  returns Ellptic Jacobi inverse CD function
*/
extern __fx64 __icd__( __fx64 cd , __fx64 x )
{
    __fx64 a[ __ITERATIONS_NUMBER__ + 1] , g[ __ITERATIONS_NUMBER__ + 1] , s[ __ITERATIONS_NUMBER__ + 1];
    __fx64 two_n , phi , phi_old , k;
    __ix32 n = 0;

    // initialization:
    k = fabs(x);
    a[0] = 1.0L;
    g[0] = sqrt(1.0 - k * k);

    // forward substitution:
    two_n = 1.0L;
    for (n = 0; n < __ITERATIONS_NUMBER__ - 1; n++)
    {
        s[n] = 0;
        two_n += two_n;
        a[n + 1] = 0.5 * (a[n] + g[n]);
        g[n + 1] = sqrt(a[n] * g[n]);
    }

    // backward substitution:
    phi = acos(sqrt(cd*cd*(x*x-1)/(cd*cd*x*x-1)));
    for (n = 1; n < __ITERATIONS_NUMBER__; n++)
    {
        phi_old = phi;
        phi = atan(g[n - 1] / a[n - 1] * tan(phi)) + phi;
        if (fabs(2 * phi_old - phi) < 5 * PI0 / 180)
        {
            s[n] = 2 * s[n - 1];
        }
        else if (fabs(2 * phi_old - phi) > PI_2)
        {
            s[n] = 2 * s[n - 1] + 1;
        }
    }

    return +(phi + s[n - 1] * PI0) / two_n / a[n - 1];
}

/*!
 * Elliptic Jacobi inverse SD function\
 * \param[dn] Elliptic Jacobi SD function
 * \param[k] elliptical modulus
 * \return  returns Ellptic Jacobi inverse SD function
*/
extern __fx64 __isd__( __fx64 sd , __fx64 x )
{
    __fx64 a[ __ITERATIONS_NUMBER__ + 1 ] , g[ __ITERATIONS_NUMBER__ + 1 ] , s[ __ITERATIONS_NUMBER__ + 1 ];
    __fx64 two_n , phi , phi_old , k;
    __ix32 n = 0;

    // initialization:
    k = fabs(x);
    a[0] = 1.0L;
    g[0] = sqrt(1.0 - k * k);

    // forward substitution:
    two_n = 1.0L;
    for (n = 0; n < __ITERATIONS_NUMBER__ - 1; n++)
    {
        s[n] = 0;
        two_n += two_n;
        a[n + 1] = 0.5 * (a[n] + g[n]);
        g[n + 1] = sqrt(a[n] * g[n]);
    }

    // backward substitution:
    phi = asin(sqrt(sd*sd / (1 + sd * sd*k*k)));
    for (n = 1; n < __ITERATIONS_NUMBER__; n++)
    {
        phi_old = phi;
        phi = atan(g[n - 1] / a[n - 1] * tan(phi)) + phi;
        if (fabs(2 * phi_old - phi) < 5 * PI0 / 180)
        {
            s[n] = 2 * s[n - 1];
        }
        else if (fabs(2 * phi_old - phi) > PI_2)
        {
            s[n] = 2 * s[n - 1] + 1;
        }
    }

    return  ( sd < 0 ) ? ( -(phi + s[n - 1] * PI0) / two_n / a[n - 1] ) : ( +(phi + s[n - 1] * PI0) / two_n / a[n - 1] ) ;
}

/*!
* Elliptic Jacobi inverse ND function
* \param[dn] Elliptic Jacobi ND function
* \param[k] elliptical modulus
* \return  returns Ellptic Jacobi inverse ND function
*/
extern __fx64 __ind__( __fx64 nd , __fx64 x ) { return __idn__( 1 / nd , x ); }

/*!
* Elliptic Jacobi inverse DC function
* \param[dn] Elliptic Jacobi DC function
* \param[k] elliptical modulus
* \return  returns Ellptic Jacobi inverse DC function
*/
extern __fx64 __idc__( __fx64 dc , __fx64 x )
{
    __fx64 a[ __ITERATIONS_NUMBER__ + 1 ] , g[ __ITERATIONS_NUMBER__ + 1 ] , s[ __ITERATIONS_NUMBER__ + 1 ];
    __fx64 two_n , phi , phi_old , k;
    __ix32 n = 0;

    // initialization:
    k = fabs(x);
    a[0] = 1.0L;
    g[0] = sqrt(1.0 - k * k);

    // forward substitution:
    two_n = 1.0L;
    for (n = 0; n < __ITERATIONS_NUMBER__ - 1; n++)
    {
        s[n] = 0;
        two_n += two_n;
        a[n + 1] = 0.5 * (a[n] + g[n]);
        g[n + 1] = sqrt(a[n] * g[n]);
    }

    // backward substitution:
    phi = acos(sqrt((1-k*k)/(dc*dc-k*k)));
    for (n = 1; n < __ITERATIONS_NUMBER__ ; n++)
    {
        phi_old = phi;
        phi = atan(g[n - 1] / a[n - 1] * tan(phi)) + phi;

        if (fabs(2 * phi_old - phi) < 5 * PI0 / 180)
        {
            s[n] = 2 * s[n - 1];
        }
        else if (fabs(2 * phi_old - phi) > PI_2)
        {
            s[n] = 2 * s[n - 1] + 1;
        }
    }

    return  +(phi + s[n - 1] * PI0) / two_n / a[n - 1];
}

/*!
* Elliptic Jacobi inverse NC function
* \param[dn] Elliptic Jacobi NC function
* \param[k] elliptical modulus
* \return  returns Ellptic Jacobi inverse NC function
*/
extern __fx64 __inc__( __fx64 nc , __fx64 x ) { return  __icn__( 1 / nc , x ); }

/*!
* Elliptic Jacobi inverse SC function
* \param[dn] Elliptic Jacobi SC function
* \param[k] elliptical modulus
* \return  returns Ellptic Jacobi inverse SC function
*/
extern __fx64 __isc__( __fx64 sc , __fx64 x )
{
    __fx64 a[ __ITERATIONS_NUMBER__ + 1 ] , g[ __ITERATIONS_NUMBER__ + 1 ] , s[ __ITERATIONS_NUMBER__ + 1 ];
    __fx64 two_n , phi , phi_old , k;
    __ix32 n = 0;

    // initialization:
    k = fabs(x);
    a[0] = 1.0L;
    g[0] = sqrt(1.0 - k * k);

    // forward substitution:
    two_n = 1.0L;
    for (n = 0; n < __ITERATIONS_NUMBER__ - 1; n++)
    {
        s[n] = 0;
        two_n += two_n;
        a[n + 1] = 0.5 * (a[n] + g[n]);
        g[n + 1] = sqrt(a[n] * g[n]);
    }

    // backward substitution:
    phi = atan(sc);
    for (n = 1; n < __ITERATIONS_NUMBER__ ; n++)
    {
        phi_old = phi;
        phi = atan(g[n - 1] / a[n - 1] * tan(phi)) + phi;
        if (fabs(2 * phi_old - phi) < 5 * PI0 / 180)
        {
            s[n] = 2 * s[n - 1];
        }
        else if (fabs(2 * phi_old - phi) > PI_2)
        {
            s[n] = 2 * s[n - 1] + 1;
        }
    }

    return  ( sc < 0 ) ? ( -(phi + s[n - 1] * PI0) / two_n / a[n - 1] ) : ( +(phi + s[n - 1] * PI0) / two_n / a[n - 1] ) ;
}

/*!
* Elliptic Jacobi inverse NS function
* \param[dn] Elliptic Jacobi NS function
* \param[k] elliptical modulus
* \return  returns Ellptic Jacobi inverse NS function
*/
extern __fx64 __ins__( __fx64 ns , __fx64 x ) { return  __isn__( 1 / ns , x ); }

/*!
* Elliptic Jacobi inverse DS function
* \param[dn] Elliptic Jacobi DS function
* \param[k] elliptical modulus
* \return  returns Ellptic Jacobi inverse DS function
*/
extern __fx64 __ids__( __fx64 ds , __fx64 x ) { return  __isd__( 1 / ds , x ); }

/*!
* Elliptic Jacobi inverse CS function
* \param[dn] Elliptic Jacobi CS function
* \param[k] elliptical modulus
* \return  returns Ellptic Jacobi inverse CS function
*/
extern __fx64 __ics__( __fx64 cs , __fx64 x ) { return  __isc__( 1 / cs , x ); }

/*!
* Elliptic integral of the first kind
* \param[k] elliptical modulus
* \return  returns Ellptic integral of the first kind
*/
extern __fx64 __ellip_k__( __fx64 k )
{
    __fxx64 a[ __ITERATIONS_NUMBER__ + 1 ] , g[ __ITERATIONS_NUMBER__ + 1 ];
    __fxx64 two_n;
    __ix32  n;

    k = fabsl( ( __fxx64 )k);

    // if k is out of range [-1 ; +1] then the edge value is set:
    if      ( k < -1 ) k = -1;
    else if ( k > +1 ) k = +1;

    // arithmetico-geometric mean method variables initialization:
    a[0] = 1.0L;
    g[0] = sqrtl(1.0L - k * k);

    /*
       Gauss arithmetic operations are continued untill the sequence converges to
       the answer that satisfies machinery epsilon accuracy
    */

    two_n = 1.0L;
    for (n = 0; n < __ITERATIONS_NUMBER__ ; n++)
    {
        if (fabsl(a[n] - g[n]) < (a[n] * __EPSILON_xx64__ )) break;
        two_n += two_n;
        a[n + 1] = 0.5L * (a[n] + g[n]);
        g[n + 1] = sqrtl(a[n] * g[n]);
    }

    return PI_2 / a[n];
}

/*!
* Elliptic integral of the second kind
* \param[k] elliptical modulus
* \return  returns Ellptic integral of the second kind
*/
extern __fx64 __ellip_e__( __fx64 k )
{
    __fxx64 a[ __ITERATIONS_NUMBER__ + 1 ] ,  g[ __ITERATIONS_NUMBER__ + 1 ] , c[ __ITERATIONS_NUMBER__ + 1 ];
    __fxx64 two_n = 1.0L , sum = 0;
    __ix32  n;

    k = fabsl( ( __fxx64 ) k );

    // if k is out of range [-1 ; +1] then the edge value is set:
    if      ( k < -1 ) k = -1;
    else if ( k > +1 ) k = +1;

    // arithmetico-geometric mean method variables initialization:
    a[0] = 1.0L;
    g[0] = sqrtl(1.0L - k * k);
    c[0] = k;

    /*
       Gauss arithmetic operations are continued untill the sequence converges to
       the answer that satisfies machinery epsilon accuracy
    */

    for (n = 0; n < __ITERATIONS_NUMBER__ ; n++)
    {
        if (fabsl(a[n] - g[n]) < (a[n] * __EPSILON_xx64__ ) ) break;
        two_n += two_n;
        a[n + 1] = 0.5L * (a[n] + g[n]);
        g[n + 1] = sqrtl(a[n] * g[n]);
        c[n + 1] = 0.5L * (a[n] - g[n]);
        sum = 0.5 * c[n + 1] * c[n + 1] * two_n + sum;
    }

    return PI_2*(1 - (sum + 0.5 * c[0] * c[0]))/ a[n];
}

/*! @} */


/*! \defgroup <Bessel_fcn> ( Bessel functions )
 *  \brief the module collaborates Bessel functions and factorial
    @{
*/

/*!
* Factorial computation function
* \param[n] input n
* \return  returns n!
*/

extern __fxx64 __factorial__( __ix64 n )
{
    //return (n == 1 || n == 0) ? 1 : __factorial__(n - 1) * n;
    __fxx64 out = 1;
    for( __ix64 i = 1 ; i <= n ; i++ ) out *= i;
    return out;
}

/*!
* Integer gamma function
* \param[n] input n
* \return  returns ( n - 1 )!
*/
extern __uix64 __gamma_integer__( __uix64 n ) { return __factorial__(n-1); }

/*!
* Zero order Bessel function
* \param[x] input x
* \return  returns zero order Bessel function value
*/
extern __fxx64 __bessel_i0__( __fxx64 x )
{
    __fxx64 sum = 0 , fact_n = 0;
    for ( __ix32 n = 0 ; n < __ITERATIONS_NUMBER__ ; n++)
    {
        fact_n = powl( __factorial__( n ), 2);
        if ( fact_n == 0 ) break;
        sum = sum + pow(-1, n) * pow(x / 2, 2 * n) / fact_n;
    }

    return sum;
}

/*!
* First order Bessel function
* \param[x] input x
* \return  returns first order Bessel function value
*/
extern __fxx64 __bessel_i1__( __fxx64 x )
{
    __fxx64 sum = 0 , fact_n = 0 , fact_n_1 = 0;
    for (__ix32 n = 0; n < __ITERATIONS_NUMBER__ ; n++)
    {
        fact_n   = __factorial__(n);
        fact_n_1 = __factorial__(n + 1);
        if (fact_n == 0 || fact_n_1 == 0) break;
        sum = sum + pow(-1, n) * powl(x / 2, 2 * n) / fact_n / fact_n_1;
    }
    return sum;
}

/*!
* N-th order Bessel function
* \param[x] input x
* \param[n] Bessel function order
* \return  returns N-th order Bessel function value
*/
extern __fxx64 __bessel_in__( __fxx64 x  , __ix32 order )
{
    __fxx64 sum = 0 , fact_n = 0 , fact_n_order = 0;
    for ( __ix32 n = 0; n < __ITERATIONS_NUMBER__ ; n++)
    {
        fact_n       = __factorial__(n);
        fact_n_order = __factorial__(n + order);
        if (fact_n == 0 || fact_n_order == 0) break;
        sum = sum + powl(-1, n) * powl(x / 2, 2 * n + order) / fact_n / fact_n_order;
    }
    return sum;
}

/*!
* N-th order modified Bessel function
* \param[x] input x
* \param[n] Modified Bessel function order
* \return  returns N-th order modified Bessel function value
*/
extern __fxx64 __modified_bessel_in__( __fxx64 x  , __ix32 order )
{
    __fxx64  sum = 0 , gamma1 = 0 , gamma2    = 0;
    for (int n = 0; n < 16; n++)
    {
        if ( ( __fx64 )gamma2 / (__fx64)__max_fx64 > 0.9 ) break;
        gamma1    = __gamma_integer__(n + 1);
        gamma2    = __gamma_integer__(n + order + 1);
        sum       = sum + powl(x / 2, 2 * n + order) / gamma1 / gamma2;
    }
    return sum;
}

/*! @} */

/*! \defgroup <Window_fcn> ( Window functions )
 *  \brief the module describes windows functions class
    @{
*/

/*!
* \brief Window function class
* \details Window function class implements the range of window functions coefficients computation and storing
*/
class wind_fcn
{
    typedef bool __bool ;
protected:

    /*! \brief window function order */
    __fx64 m_order;

    /*! \brief window function completness flag */
    __bool m_wind_ready;

    /*! \brief window function coefficients buffer */
    __fx64 *m_buff;
public:

    // constructor and destructor:
    wind_fcn()
    {
        m_wind_ready = 0;
        m_buff       = 0;
    }

    ~wind_fcn() { deallocate(); }

    /*!
     * \brief window function class object initialization function
     * \param[order] window function length ( in samples )
    */
    void init( __ix32 order )
    {
        m_order = order;
        m_buff  = 0;
    }

    /*!
     * \brief window function class object memory allocation function
     * \details the function allocates coefficients buffer ( *m_buff )
     *          of the window function class object
    */
    __ix32 allocate()
    {
        m_buff = ( __fx64* ) calloc( m_order , sizeof ( __fx64 ) );
        return ( !m_buff );
    }

    /*!
     * \brief window function class object memory deallocation function
     * \details the function frees coefficients buffer ( *m_buff )
     *          of the window function class object. Buffer memory
     *          is freed if the buffer has not been freed earlier.
    */
    void deallocate()
    {
        if ( m_buff != 0 )
        {
            free( m_buff );
            m_buff = 0;
        }
    }

    /*!
     * \brief Barlett window function computation
     * \return The function allocates memory sets, m_wind_ready = 1 and computes Barlett window coefficients as follows:
     *  \f[
     *      Ns = order \newline
     *      n = 0 \dots Ns \newline
     *      y( n ) = \begin{equation}
     *      \begin{cases}
     *      2 * n / ( Ns - 1 ) , n <= ( Ns - 1 ) / 2
     *      \\
     *      2 - 2 * n / ( Ns - 1 ) , n > ( Ns - 1 ) / 2
     *      \end{cases}
     *      \end{equation}
     *  \f]
    */
    __ix32 Bartlett()
    {
        // memory allocation:
        allocate();

        // window function coefficients computation:
        __fx64 Ns = ( __fx64 ) m_order;
        for ( __ix32 n = 0; n < Ns ; n++) { m_buff[n] = ( n <= ( Ns-1 ) / 2 ) ? ( 2*n / (Ns-1) ) : ( 2 - 2 * n / (Ns-1) ); }

        // window is ready to use:
        m_wind_ready = 1;
        return m_wind_ready;
    }

    /*!
     * \brief Barlett Hanning window function computation
     * \return The function allocates memory, sets m_wind_ready = 1 and computes Barlett-Hanning window coefficients as follows:
     *  \f[
     *      Ns = order \newline
     *      n  = 0 \dots Ns \newline
     *      y( n ) = 0.62 - 0.48 * \left|  \frac{ n }{ Ns - 1 } - 0.5 \right| + 0.38 * cos \left[ \ 2 * \pi * \left( \frac{ n }{ Ns - 1 } - 0.5 \right) \right]
     *  \f]
    */

    __ix32 BartlettHanning()
    {
        // memory allocation:
        allocate();

        // window function coefficients computation:
        __fx64 Ns = (__fx64)m_order;
        for (__ix32 n = 0; n < Ns ; n++) { m_buff[n] = 0.62 - 0.48 * fabs( n / (Ns-1)-0.5) + 0.38 * cos(PI2 * (n / (Ns - 1) - 0.5)); }

        // window is ready to use:
        m_wind_ready = 1;
        return m_wind_ready;
    }

    /*!
     * \brief Balckman window function computation
     * \return The function allocates memory, sets m_wind_ready = 1 and computes Balckman window coefficients as follows:
     *  \f[
     *      Ns = order \newline
     *      n  = 0 \dots Ns \newline
     *      y( n ) = 0.42 - 0.50 * cos \left( 2 * \pi * \frac { n } { Ns - 1 } \right ) + 0.08 * cos \left( 4 * \pi * \frac{ n } { Ns - 1 } \right )
     *  \f]
    */
    __ix32 Blackman()
    {
        // memory allocation:
        allocate();

        // window function coefficients computation:
        __fx64 Ns = (__fx64)m_order;
        for (__ix32 n = 0; n < Ns ; n++) { m_buff[n] = 0.42 - 0.50 * cos(2 * PI0 * n / (Ns - 1) ) + 0.08 * cos(4 * PI0 * n / (Ns - 1)); }

        // window is ready to use:
        m_wind_ready = 1;

        return 0;
    }

    /*!
     * \brief Balckman-Harris window function computation
     * \return The function allocates memory, sets m_wind_ready = 1 and computes Balckman-Harris window coefficients as follows:
     *  \f[
     *      Ns = order \newline
     *      n  = 0 \dots Ns \newline
     *      y( n ) = 0.35875 - 0.48829 * cos \left( 2 * \pi * \frac{ n } { Ns - 1 } \right ) + 0.14128 * cos\left( 4 * \pi * \frac{ n } { Ns - 1 } \right ) - 0.01168 * cos \left( 6 * \pi * \frac{ n } { Ns - 1 } \right )
     *  \f]
    */
    __ix32 BlackmanHarris()
    {
        // memory allocation:
        allocate();

        // window function coefficients computation:
        __fx64 Ns = (__fx64)m_order;
        for (__ix32 n = 0; n < Ns; n++) { m_buff[n] = 0.35875 - 0.48829 * cos(2 * PI0 * n / (Ns - 1)) + 0.14128 * cos(4 * PI0 * n / (Ns - 1)) - 0.01168 * cos(6 * PI0 * n / (Ns - 1)); }

        // window is ready to use:
        m_wind_ready = 1;
        return m_wind_ready;
    }

    /*!
     * \brief Bohman window function computation
     * \return The function allocates memory, sets m_wind_ready = 1 and computes Bohman window coefficients as follows:
     *  \f[
     *      Ns = ceil \left( \frac{ order } { 2 } \right ) \quad , \quad n  = 0 \dots Ns \newline
     *      y( n ) = \left( 1 - \left| \frac{ n } { Ns + 1} \right| \right) * cos \left( \pi * \left| \frac{ n } { Ns + 1 } \right| \right ) + \frac{ 1 } { \pi } * sin \left( \pi * \left| \frac { n } { Ns + 1 } \right| \right) \quad , \quad
     *      n = \begin{cases}
     *      n = k - Ns \quad \quad \quad , \quad n = 0
     *      \\
     *      n = k - Ns - 1 \quad , \quad n >= 0
     *      \end{cases}
     *  \f]
    */
    __ix32 Bohman()
    {
        // memory allocation:
        allocate();

        // window function coefficients computation:
        __fx64 Ns = ceil( (__fx64)m_order / 2) , n = 0;
        for (__ix32 k = 0; k < m_order; k++)
        {
            n = k - Ns;
            if ( n >= 0 )  n = k - Ns + 1;
            m_buff[k] = ( 1 - fabs( n / (Ns + 1) ) ) * cos( PI0 * fabs(n / (Ns + 1))) + 1 / PI0 * sin(PI0 * fabs(n / (Ns + 1)));
        }

        // window is ready to use:
        m_wind_ready = 1;
        return m_wind_ready;
    }

    /*!
     * \brief Auxiliary Chebhshev poly computation function
     * \param[n] Chebyshev poly order
     * \param[x] x-value at which n-th order Chebyshev poly value is computed
     * \return The function returns Chebyshev n-th order poly value
     *  \f[
     *      y( x , n ) =
     *      \begin{cases}
     *      cos \left( n * acos \left( x \right ) \right ) \quad , \quad |x| <= 1
     *      \\
     *      cos \left( n * acosh \left( x \right ) \right ) \quad , \quad |x| > 1
     *      \end{cases}
     *  \f]
    */
    __fx64 cheby_poly(__ix32 n, __fx64 x)
    {
        __fx64 res;
        if (fabs(x) <= 1) res = cos(n*acos(x));
        else              res = cosh(n*acosh(x));
        return res;
    }

    /*!
     * \brief Chebyshev window computation function
     * \param[ atten ] sidelobe attenuation , Db
     * \return The function allocates memory, sets m_wind_ready = 1 and computes Chebyshev window coefficients
    */
    __ix32 Chebyshev( __fx64 atten )
    {
        // memory allocation:
        allocate();

        // window function coefficients computation:
        __ix32 Ns = m_order , nn, kk;
        __fx64 M, n, sum = 0, max = 0;
        __fx64 tg = pow(10, atten / 20);
        __fx64 x0 = cosh((1.0 / (Ns - 1))*acosh(tg));
        M = (Ns - 1) / 2;

        if (Ns % 2 == 0) M = M + 0.5;
        for (nn = 0; nn < (Ns / 2 + 1); nn++)
        {
            n = nn - M;
            sum = 0;
            for (kk = 1; kk <= M; kk++)  { sum += cheby_poly(Ns - 1, x0*cos(PI0*kk / Ns))*cos(2.0*n*PI0*kk / Ns); }
            m_buff[nn] = tg + 2 * sum;
            m_buff[(__ix32)Ns - nn - 1] = m_buff[nn];
            if (m_buff[nn] > max)max = m_buff[nn];
        }
        for (nn = 0; nn < Ns; nn++) m_buff[nn] /= max;

        // window is ready to use:
        m_wind_ready = true;
        return m_wind_ready;
    }

    /*!
     * \brief Flat-Top window computation function
     * \return The function allocates memory, sets m_wind_ready = 1 and computes Flat-Top window coefficients as follows:
     *  \f[
     *      Ns = order \newline
     *      n  = 0 \dots Ns \newline
     *      y( n ) = 0.21557895 - 0.41663158 * cos \left( ( 2 * \pi * \frac{ n } { Ns - 1 } \right)
     *      + 0.277263158 * cos \left( 4 * \pi * \frac{ n } { Ns - 1 } \right)
     *      - 0.083578947 * cos \left( 6 * \pi * \frac{ n } { Ns - 1 } \right)
     *      + 0.006947368 * cos \left( 8 * \pi * \frac{ n } { Ns - 1 } \right)
     *  \f]
    */
    __ix32 FlatTop()
    {
        // memory allocation:
        allocate();

        // window coefficients computation:
        __fx64 Ns = (__fx64)m_order;
        for (__ix32 n = 0 ; n < Ns; n++) { m_buff[n] = 0.21557895 - 0.41663158 * cos(PI2 * n / (Ns - 1) ) + 0.277263158 * cos(4 * PI0 * n / (Ns - 1) ) - 0.083578947 * cos(6 * PI0 * n / (Ns - 1) ) + 0.006947368 * cos(8 * PI0 * n / (Ns - 1) ); }

        // window is ready to use:
        m_wind_ready = true;
        return m_wind_ready;
    }

    /*!
     * \brief Gaussian window computation function
     * \param[ alpha ] Gaussian window parameter
     * \return The function allocates memory, sets m_wind_ready = 1 and computes Gaussian window coefficients as follows:
     *  \f[
     *      Ns = order      \newline
     *      \sigma = 0      \newline
     *      n  = 0 \dots Ns \newline
     *      y( n ) = e^ \left( \frac{ n^2 }{ 2 * \sigma^2 } \right) \newline
     *      \sigma = \frac{ Ns-1 }{ 2 * \alpha } \newline
     *      n =
     *      \begin{cases}
     *      k - \frac{ Ns }{ 2 } \quad , \quad n < 0
     *      \\
     *      k - \frac{ Ns }{ 2 } + 1 \quad , \quad n \geq 0
     *      \end{cases}
     *  \f]
    */
    __ix32 Gaussian( __fx64 alpha )
    {
        // memory allocation:
        allocate();

        // window function coefficients computation:
        __ix32 Ns = m_order;
        __ix32 n     = 0;
        __fx64 sigma = 0;
        for (__ix32 k = 0 ; k < Ns; k++)
        {
            n = k - Ns/2;
            if (n >= 0)  n = k - Ns/2 + 1;
            sigma = ((__fx64)Ns-1) / 2 / alpha;
            m_buff[k] = exp(-(__fx64)n * (__fx64)n / 2 / sigma / sigma);
        }

        // wind is ready to use:
        m_wind_ready = true;
        return m_wind_ready;
    }

    /*!
     * \brief Hamming window computation function
     * \return The function allocates memory, sets m_wind_ready = 1 and computes Hamming window coefficients as follows:
     *  \f[
     *      Ns = order      \newline
     *      n  = 0 \dots Ns \newline
     *      y( n ) = 0.54 - 0.46 * cos \left( 2 * \pi * \frac{ n } { Ns-1 } \right)
     *  \f]
    */
    __ix32 Hamming()
    {
        // memory allocation:
        allocate();

        // window function coefficients computation:
        __ix32 Ns = m_order;
        for (__ix32 n = 0 ; n < Ns ; n++)  { m_buff[n] = 0.54 - 0.46 * cos(PI2 * n / (Ns-1)); }

        // window is ready to use:
        m_wind_ready = true;
        return m_wind_ready;
    }

    /*!
     * \brief Hann window computation function
     * \return The function allocates memory, sets m_wind_ready = 1 and computes Hann window coefficients as follows:
     *  \f[
     *      Ns = order      \newline
     *      n  = 0 \dots Ns \newline
     *      y( n ) = 0.5 - 0.5 * cos \left( 2 * \pi * \frac{ n } { Ns-1 } \right)
     *  \f]
    */
    __ix32 Hann()
    {
        // memory allocation:
        allocate();

        // window function coefficients computation:
        __ix32 Ns = m_order;
        for (__ix32 n = 0; n < Ns; n++) { m_buff[n] = 0.5 - 0.5 * cos( PI2 * n / (Ns-1) ); }

        // window is ready to use:
        m_wind_ready = true;
        return m_wind_ready;
    }

    /*!
     * \brief Kaiser window computation function
     * \param[betta] Kaiser window function parameter
     * \return The function allocates memory, sets m_wind_ready = 1 and computes Kaiser window coefficients as follows:
     *  \f[
     *      Ns = order                                                                      \newline
     *      n  = 0 \dots Ns                                                                 \newline
     *      A  = \frac{ \left( n - \frac{ Ns - 1 } { 2 } \right) }{ \frac{ Ns - 1 } { 2 } } \newline
     *      B  = \beta * \sqrt( 1 - A^2 )                                                   \newline
     *      C  = \beta                                                                      \newline
     *      y(n) = \frac{ ModifiedBessel( B , 0 ) }{ ModifiedBessel( C , 0 ) }
     *  \f]
    */
    __ix32 Kaiser( __fx64 betta )
    {
        // memory allocation:
        allocate();

        // winfow function coefficients computation:
        __ix32 Ns = m_order;
        __fx64 A  = 0;
        __fx64 B  = 0;
        __fx64 C  = 0;
        for (__ix32 n = 0; n < Ns; n++)
        {
            A = ((__fx64)n - ( (__fx64)Ns - 1 ) / 2) / ( ((__fx64)Ns - 1 ) / 2);
            B = betta * sqrt(1 - A * A);
            C = betta;
            m_buff[n] = __modified_bessel_in__( B , 0 ) / __modified_bessel_in__( C , 0 );
        }

        // window is ready to use:
        m_wind_ready = true;
        return m_wind_ready;
    }

    /*!
     * \brief Nutall window computation function
     * \return The function allocates memory, sets m_wind_ready = 1 and computes Nutall window coefficients as follows:
     *  \f[
     *      Ns = order      \newline
     *      n  = 0 \dots Ns \newline
     *      y(n) = 0.3635819
     *      - 0.4891775 * cos \left( 2 * \pi * \frac{ n }{ Ns - 1 } \right)
     *      + 0.1365995 * cos \left( 4 * \pi * \frac{ n }{ Ns - 1 } \right)
     *      - 0.0106411 * cos \left( 6 * \pi * \frac{ n }{ Ns - 1 } \right)
     *  \f]
    */
    __ix32 Nutall()
    {
        // memory allocation:
        allocate();

        // window function coefficients computation:
        __ix32 Ns = m_order;
        for (__ix32 n = 0; n < Ns; n++)
        {
            m_buff[n] = 0.3635819 - 0.4891775 * cos(2 * PI0 * n / (Ns - 1) ) + 0.1365995 * cos(4 * PI0 * n / (Ns - 1)) - 0.0106411 * cos(6 * PI0 * n / (Ns - 1));
        }

        // window is ready to use:
        m_wind_ready = true;
        return m_wind_ready;
    }

    /*!
     * \brief Parzen window computation function
     * \return The function allocates memory, sets m_wind_ready = 1 and computes Parzen window coefficients as follows:
     *  \f[
     *      Ns = order      \newline
     *      n  = 0 \dots Ns \newline
     *      y(n) =
     *      \begin{cases}
     *
     *      1 - 6 * \left( \frac{ |n| } { \left( Ns / 2 \right) } \right)^2
     *      + 6 * \left( \frac{ |n| } { \left( Ns / 2 \right) } \right)^3 \quad , \quad 0 \leq |n| \leq \frac{ Ns - 1 }{ 4 }
     *      \\
     *      2 * \left( \frac{ |n| } { \left( Ns / 2 \right) } \right)^3 \quad  , \quad \frac{ Ns - 1 }{ 4 } < |n| \leq \frac{ Ns - 1 }{ 2 }
     *
     *      \end{cases}
     *
     *  \f]
    */
    __ix32 Parzen()
    {
        // memory allocation:
        allocate();

        // window function coefficients computation:
        __ix32    Ns = m_order;
        __ix32    n = 0;
        for (__ix32 k = 0; k < Ns; k++)
        {
            n = k - Ns / 2;
            if (n >= 0)  n = k - Ns / 2 + 1;

            if ( abs(n)>=0 && abs(n) <= (Ns-1)/4 )
            {
                m_buff[k] = 1 - 6 * fabs((__fx64)n)*fabs((__fx64)n) / ((__fx64)Ns*(__fx64)Ns / 4) + 6 * fabs((__fx64)n)*fabs((__fx64)n)*fabs((__fx64)n) / ((__fx64)Ns*(__fx64)Ns*(__fx64)Ns / 8);
            }
            else if( abs(n) < Ns/2 && abs(n) > (Ns - 1) / 4 )
            {
                m_buff[k] = 2 * pow( 1 - fabs((__fx64)n)/((__fx64)Ns/2) , 3);
            }
        }

        // window is ready:
        m_wind_ready = true;
        return m_wind_ready;
    }

    /*!
     * \brief Rectangular window computation function
     * \return The function allocates memory, sets m_wind_ready = 1 and computes Rectangular window coefficients as follows:
     *  \f[
     *      Ns = order      \newline
     *      n  = 0 \dots Ns \newline
     *      y(n) = 1
     *  \f]
    */
    __ix32 Rectangular()
    {
        // memory allocation:
        allocate();

        // window coefficients vomputation:
        for (__ix32 n = 0; n < m_order; n++)  m_buff[n] = 1;

        // window is ready to use:
        m_wind_ready = true;
        return m_wind_ready;
    }

    /*!
     * \brief Triangular window computation function
     * \return The function allocates memory, sets m_wind_ready = 1 and computes Triangular window coefficients as follows:
     *  \f[
     *      Ns = order      \newline
     *      n  = 0 \dots Ns \newline
     *      y(n) =
     *      \begin{cases}
     *      1 - \left| \frac{ n - \left( Ns - 1 \right) / 2 }{ \left( Ns / 2 \right) } \right| \quad , \quad Ns \mod 2 = 0
     *      \\
     *      1 - \left| \frac{ n - \left( Ns - 1 \right) / 2 }{ \left( ( Ns + 1 ) / 2 \right) } \right| \quad , \quad Ns \mod 2 \neq 0
     *      \end{cases}
     *  \f]
    */
    __ix32 Triangular()
    {
        // memory allocation:
        allocate();

        // window function coefficients computation:
        __ix32 Ns = m_order;
        if ( Ns % 2 == 0)
        {
            for (__ix32 n = 0; n < Ns; n++) { m_buff[n] = (1.0 - fabs(((__fx64)n - ((__fx64)Ns - 1) / 2) / ((((__fx64)Ns - 1) + 1) / 2))); }
        }
        else
        {
            for (__ix32 n = 0; n < Ns; n++) { m_buff[n] = (1.0 - fabs(((__fx64)n - ((__fx64)Ns - 1) / 2) / ((((__fx64)Ns - 1) + 2) / 2))); }
        }

        // window is ready to use:
        m_wind_ready = true;
        return m_wind_ready;
    }

    /*!
     * \brief Tukey window computation function
     * \param[R] Tukey window function parameter
     * \return The function allocates memory, sets m_wind_ready = 1 and computes Tukey window coefficients as follows:
     *  \f[
     *      Ns = order      \newline
     *      n  = 0 \dots Ns \newline
     *      y \left( x = \frac{ n }{ Ns - 1 } \right) =
     *      \begin{cases}
     *      0.5 + 0.5 * cos \left( \frac{ 2 * \pi }{ R * ( x - 0.5*R ) } \right) \quad , \quad  0 \geq x < R / 2;
     *
     *      \\
     *
     *      0.5 + 0.5 * cos \left( \frac{ 2 * \pi }{ R * ( x - 1 + 0.5*R ) } \right) \quad , \quad  1 - 0.5*R \geq x \leq 1;
     *
     *      \\
     *
     *      1
     *
     *      \end{cases}
     *  \f]
    */
    __ix32 Tukey( __fx64 R )
    {
        // memory allocation:
        allocate();

        // window function coefficients computation:
        __ix32 Ns = m_order;
        __fx64 x  = 0;
        for(__ix32 n = 0 ; n < Ns ; n++)
        {
            x = (__fx64)n / ((__fx64)Ns - 1);
            if ( x >=0 && x < R / 2 )
            {
                m_buff[n] = 0.5 + 0.5 * cos( PI2 / R * ( x - 0.5*R ) );
            }
            else if ( x >= 1 - 0.5 * R && x <= 1 )
            {
                m_buff[n] = 0.5 + 0.5 * cos(PI2 / R * (x - 1 + 0.5*R) );
            }
            else
            {
                m_buff[n] = 1;
            }
        }

        // window is ready to use:
        m_wind_ready = true;
        return m_wind_ready;
    }

    /*!
     * \brief the function returns m_wind_ready flag
    */
    bool is_ready() { return m_wind_ready; }

    /*!
     * \brief operator extracts n-th value out of woindow function object buffer
    */
    inline __fx64 operator [] ( __ix32 n ) { return m_buff[n]; }
};

/*! @} */

// customized types names exclusion to avloid aliasing during compilation:
#undef __ix16
#undef __ix32
#undef __ix64
#undef __uix16
#undef __uix32
#undef __uix64
#undef __fx32
#undef __fx64
#undef __fxx64

// customized upper limits exclusion to avloid aliasing during compilation:
#undef __max_fx32
#undef __max_fx64
#undef __max_ix16
#undef __max_ix32
#undef __max_ix64
#undef __max_uix16
#undef __max_uix32
#undef __max_uix64

// customized lower limits exclusion to avloid aliasing during compilation:
#undef __min_fx32
#undef __min_fx64
#undef __min_ix16
#undef __min_ix32
#undef __min_ix64
#undef __min_uix16
#undef __min_uix32
#undef __min_uix64

// customized epsilon exclusion to avloid aliasing during compilation:
#undef __EPSILON_xx64__
#undef __EPSILON_x64__
#undef __EPSILON_x32__

#undef __TO_DEGREES
#undef __TO_RADIANS

// customized pi undef:
#undef PI0
#undef PI2
#undef PI_2
#undef PI_4

// customized variables exclusion to avloid aliasing during compilation:
#undef __ITERATIONS_NUMBER__

#endif // SPECIAL_FUNCTIONS_H

//--------------------------------------------------------------------------------
//A.M.Tykvinsky, 30.09.2020
//--------------------------------------------------------------------------------
// ФИЛЬТР С КОНЕЧНОЙ ИМПУЛЬСНОЙ ХАРАКТЕРИСТИКОЙ
//--------------------------------------------------------------------------------

#ifndef _FIR_FILT_HPP
#define _FIR_FILT_HPP

#include <cmath>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "mirror_ring_buff_x32.hpp"
#include "wind_fcn.hpp"
#include "math_const.hpp"
#include "special_functions.hpp"
#include "recursive_fourier.hpp"

// ФНЧ - фильтр нижних  частот
// ФВЧ - фильтр верхних частот
// ПФ  - фильтр полосовой
// РФ  - фильтр режекторный

class FIR_filt
{

protected:

	// системные переменные класса:

	double m_Ts;        // период дискретизации входной выборки АЦП
	double m_Fs;	    // частота дискретизаци АЦП
	double m_Fn;        // номинальная частота сети
	double m_Fstop;     // частота среза
	double m_Fstop1;    // частота начала полосы пропускания/подавления
	double m_Fstop2;    // частота конца полосы пропускания/подавления
	double m_Ns;        // число выборок на период сигнала
	double m_ang;       // угол фазового сдвига
	int    m_order;     // порядок фильтра
	int    m_filt_type; // тип фильтра
	bool   m_scale;     // нормировать или ненормироать коэффициенты фильтра (true - да, нормировать, false - нет, не нормировать )

	// значения, которые может принимать переменная m_filt_type:
	// 1  - фильтр нижних  частот      (  ФНЧ  )
	// 2  - фильтр высоких частот      (  ФВЧ  )
	// 3  - полосовой фильтр           (  ПФ   )
	// 4  - режекторный фильтр         (  РФ   )
	// 5  - синусный фильтр	           ( СФ/КФ )
	// 6  - косинусный фильтр  	   ( СФ/КФ )
	// 7  - фильтр Хартли              (  ХФ   )
	// 8* - эффективная реализация

	// Буффер под коэффициенты фильтра: 
	mirror_ring_buff m_BUFF_WIND_CX;  // массив под коэффициенты числителя ПФ фильтра


	//=========================================================
	// Параметры для эффективного рекурсивного КИХ фильтра:
	//=========================================================
	struct vector
	{
		float re;
		float im;
	};
	
	vector *m_Wnum , *m_Wden;
	
	int   m_ElemNum , m_Ncplx_coeff;
	float m_a , m_a0 , m_b , m_b0, m_dx , m_Gain;
	float *m_y_re , *m_y_im;
	// =========================================================

public:

	// зеркальный буффер для отсчетов АЦП:
	mirror_ring_buff m_BUFF_WIND_SX;

	// оконная функция, проектируемая пользователем:
	wind_fcn  m_WIND_FCN;

	// входы фильтра:
	double    m_Km   , m_pH;  // коэффициенты АЧХ и ФЧХ
	double    m_W_re , m_W_im; // комплексный коэффициент передачиы
	double    m_in_F;         // текущее значение частоты

	// выход фильтра:
	double    m_out;

	//Конструктор:
	FIR_filt();

	//Деструктор:
	~FIR_filt();

	// функция расчета АЧХ и ФЧХ фильтра:
	int FreqCharacteristics();
	int FreqCharacteristics( bool mode );

	// функция расчета коэффициентов фильтра:
	int CoeffCalc();

	// функция инициализации фильтра:
	int LP_Init(double Fs, double Fn, double F_stop, int order, bool scale);                     // функция инициализации ФНЧ
	int HP_Init(double Fs, double Fn, double F_stop, int order, bool scale);                    // функция инициализации ФВЧ
	int BP_Init(double Fs, double Fn, double F_stop1, double F_stop2, int order, bool scale);  // функция инициализации ПФ
	int BS_Init(double Fs, double Fn, double F_stop1, double F_stop2, int order, bool scale); // функция инициализации РФ
	int SF_Init(double Fs, double Fn, int order);                                            // функция инициализации синусного/косинусного фильтра
	int CF_Init(double Fs, double Fn, int order);
	int HF_Init(double Fs, double Fn, double pH , int order);

	// функция выделения памяти под окно фильтра
	int allocate();
	int allocate( int Nbot , int Ntop );

	// функция освобождения памяти
	int deallocate();

	// функция отображения спецификации фильтра:
	int ShowFiltSpec();

	// функция отображения коэффициентов фильтра:
	int ShowFiltCoeffs();
	
	// функция каскадирования КИХ - фильтров (т.е. их эквивалентирования в один фильтр):
	int Cascade( float *c , int Nc );
	
	// встроенные функции ( для ускорения их вызова ):
	//------------------------------------------------------------------------------
	// Функция фильтрации ( фильтрация мгновенных значений ):
	inline int filt(double *in)
	{
		// заполняем оконный буффер фильтра:
		m_BUFF_WIND_SX.fill_buff( in );
		
		// обнуление выходного значения:
		m_out = 0;

		// фильтрация:
		for (int n = m_order; n >= 0; n--)
		{
		      m_out += m_BUFF_WIND_SX.m_ptr_fill_down[n + 1] * m_BUFF_WIND_CX.m_buff[n];
		} 

		return 0;

	}

	// Функция фильтрации ( фильтрация мгновенных значений ):
	inline int filt(float *in)
	{
		// заполняем оконный буффер фильтра:
		m_BUFF_WIND_SX.fill_buff( in );
		
		// обнуление выходного значения:
		m_out = 0;

		// фильтрация:
		for (int n = m_order; n >= 0; n--)
		{
			m_out += m_BUFF_WIND_SX.m_ptr_fill_down[n + 1] * m_BUFF_WIND_CX.m_buff[n];
		} 

		return 0;

	}

	// Функция для фильтрации выборки 1 раз за такт:
	inline int filt()
	{
		// функция предполагает, что буффер заполняется внешне (см. hpp - файл !!!)

		// обнуление выходного значения:
		m_out = 0;
		
		// фильтрация:
		for (int n = m_order; n >= 0; n--)
		{
			m_out += m_BUFF_WIND_SX.m_ptr_fill_down[n + 1] * m_BUFF_WIND_CX.m_buff[n];
		} 
		
		return 0;
	}


	// функции с использованием эффективной КИХ - фильтрации:
	inline int filt_eff( double *input )
	{

	    // заполнение буффера:
	    m_BUFF_WIND_SX.fill_buff( input );

	    // фильтрация:
	    m_dx  = *input - m_BUFF_WIND_SX.m_ptr_fill_down[ m_ElemNum ];
	    m_out = 0;

	    for( int i = 0 ; i < m_Ncplx_coeff ; i++ )
	    {
		m_a0      = m_dx * m_Wnum[i].re + ( m_y_re[i] * m_Wden[i].re - m_y_im[i] * m_Wden[i].im );
		m_y_im[i] = m_dx * m_Wnum[i].im + ( m_y_im[i] * m_Wden[i].re + m_y_re[i] * m_Wden[i].im );
		m_y_re[i] = m_a0;
		m_out    += m_a0;
	    }

	    return 0;
	}

	inline int filt_eff( float *input )
	{
	    // заполнение буффера:
	    m_BUFF_WIND_SX.fill_buff( input );

	    // фильтрация:
	    m_dx  = *input - m_BUFF_WIND_SX.m_ptr_fill_down[m_ElemNum];
	    m_out = 0;

	    for( int i = 0 ; i < m_Ncplx_coeff ; i++ )
	    {			
		m_a0      = m_dx * m_Wnum[i].re + ( m_y_re[i] * m_Wden[i].re - m_y_im[i] * m_Wden[i].im );
		m_y_im[i] = m_dx * m_Wnum[i].im + ( m_y_im[i] * m_Wden[i].re + m_y_re[i] * m_Wden[i].im );
		m_y_re[i] = m_a0;
		m_out    += m_a0;
	    }
		
	    return 0;
	}


	//------------------------------------------------------------------------------
	// Функция получения коэффициентов фильтра:
	inline float get_coeff( int n )
	{
	    return m_BUFF_WIND_CX.m_buff[n];
	}

	// функция получения порядка фильтра:
	inline int get_flt_order()
	{
		return m_order + 1;
	}

	
};

#endif
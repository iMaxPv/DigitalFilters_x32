//-----------------------------------------------------------------------------------------------------------------------------------------------
// A.M. Tykvinsky, 19.10.2020
//-----------------------------------------------------------------------------------------------------------------------------------------------

#include "aperiodic_filt.hpp"

// ФИЛЬТР АПЕРИОДИЧЕСКОЙ СЛАГАЮЩЕЙ И ВТОРОЙ ГАРМОНИКИ
//-----------------------------------------------------------------------------------------------------------------------------------------------
// конструктор по умолчанию:
aperiodic_filt::aperiodic_filt()
{
	// инициализация системных  переменных:
	m_Fs      = 4000;
	m_Fn      = 50;
	m_Ts      = 1 / m_Fs;
	m_order   = m_Fs / m_Fn / 2;
	m_ElemNum = m_order + 1;
	
	// инициализация зеркальных буфферов фильтра:
	m_MBUFF.BuffInit(m_ElemNum);
	
	// инициализация выходных переменных секций фильтра:
	m_out      = 0;
	m_Km       = 0;
	m_pH       = 0;
	m_in_F     = 50;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// деструктор:
aperiodic_filt::~aperiodic_filt() {}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// функция инициализации фильтра:
int aperiodic_filt::filtInit( double Fs , double Fn )
{
	// инициализация системных переменных фильтра:
	m_Fs      = Fs;
	m_Fn      = Fn;
	m_Ts      = 1 / m_Fs;
	m_order   = m_Fs / m_Fn / 2;
	m_ElemNum = m_order + 1;
	
	// инициализация зеркальных буфферов фильтра:
	m_MBUFF.BuffInit(m_ElemNum);
	
	// инициализация выходных переменных секций фильтра:
	m_out      = 0;
	m_Km       = 0;
	m_pH       = 0;
	m_in_F     = Fn;
	
	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// функция выделения памяти:
int aperiodic_filt::allocate()
{
	// выделение памяти под зеркальные буфферы фильтра:
	m_MBUFF.allocate(true);
	return 0;	
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// функция освобождения памяти:
int aperiodic_filt::deallocate()
{
	// освобождение памяти под зеркальных буфферов фильтра:
	m_MBUFF.deallocate();
	return 0;	
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
//функция расчета АЧХ и ФЧХ фильтра:
int aperiodic_filt::FreqCharacteristics()
{

	//Компенсация АЧХ и ФЧХ КИХ фильтра:
	double Re  = 0;
	double Im  = 0;

	Re = 1 - cos(-6.283185307179586476925286766559  * m_order * m_in_F * m_Ts);
	Im = 0 - sin(-6.283185307179586476925286766559  * m_order * m_in_F * m_Ts);

	m_pH = atan2(Im, Re);
	m_Km = sqrt(Re * Re + Im * Im) * 0.5;

	return 0;
};
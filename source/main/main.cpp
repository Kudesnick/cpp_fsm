/***************************************************************************************************
 *   Project:
 *   Author:        Stulov Tikhon (kudesnick@inbox.ru)
 ***************************************************************************************************
 *   Distribution:
 *
 ***************************************************************************************************
 *   MCU Family:    STM32F
 *   Compiler:      ARMCC
 ***************************************************************************************************
 *   File:          main.cpp
 *   Description:
 *
 ***************************************************************************************************
 *   History:       22.04.2021 - file created
 *
 **************************************************************************************************/

/***************************************************************************************************
 *                                      INCLUDED FILES
 **************************************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/***************************************************************************************************
 *                                       BASE CLASSES
 **************************************************************************************************/

class cpp_fsm;
class cpp_fsm_step;

class cpp_fsm_step
{
protected:
    cpp_fsm &fsm;
public:
    cpp_fsm_step(cpp_fsm &_fsm, bool _init = false);
    virtual void init(void) {};
    virtual void routine(void) = 0;
};

class cpp_fsm
{
protected:
    cpp_fsm_step *init_step;
    cpp_fsm_step *prev_step;
    cpp_fsm_step *step;
public:
    cpp_fsm(){};
    void set_step(cpp_fsm_step *const _step);
    void routine(void);
    void reset(void);
};

cpp_fsm_step::cpp_fsm_step(cpp_fsm &_fsm, bool _init):
    fsm(_fsm)
{
    if (_init)
    {
        fsm.set_step(this);
    }
};

void cpp_fsm::set_step(cpp_fsm_step *const _step)
{
    step = _step;
    if (init_step == nullptr)
    {
        init_step = _step;
    }
};

void cpp_fsm::routine(void)
{
    if (step != nullptr)
    {
        if (step != prev_step)
        {
            step->init();
            prev_step = step;
        }
        else
        {
            step->routine();
        }
    }
};

void cpp_fsm::reset(void)
{
    step = init_step;
}

/***************************************************************************************************
 *                                     EXAMPLE OBJECTS
 **************************************************************************************************/

cpp_fsm example_fsm = {};

class cpp_example_step_1: public cpp_fsm_step
{
protected:
    virtual void init(void) override
    {
        // something initialisation
    };
    virtual void routine(void) override;
public:
    using cpp_fsm_step::cpp_fsm_step;
} example_step_1 = {example_fsm, true};

class cpp_example_step_2: public cpp_fsm_step
{
protected:
    virtual void routine(void) override;
public:
    using cpp_fsm_step::cpp_fsm_step;
} example_step_2 = {example_fsm};

void cpp_example_step_1::routine(void)
{
    fsm.set_step(&example_step_2);
}

void cpp_example_step_2::routine(void)
{
    fsm.set_step(&example_step_1);
}

/***************************************************************************************************
 *                                    PUBLIC FUNCTIONS
 **************************************************************************************************/

#if !defined(__CC_ARM) && defined(__ARMCC_VERSION) && !defined(__OPTIMIZE__)
    /*
    Without this directive, it does not start if -o0 optimization is used and the "main"
    function without parameters.
    see http://www.keil.com/support/man/docs/armclang_mig/armclang_mig_udb1499267612612.htm
    */
    __asm(".global __ARM_use_no_argv\n\t" "__ARM_use_no_argv:\n\t");
#endif

int main(void)
{
    for(;;)
    {
        example_fsm.routine();
    }

    return 0;
}

/***************************************************************************************************
 *                                       END OF FILE
 **************************************************************************************************/

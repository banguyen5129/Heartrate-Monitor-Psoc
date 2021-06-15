/*******************************************************************************
* File Name: counter_reset_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "counter_reset.h"

/* Check for removal by optimization */
#if !defined(counter_reset_Sync_ctrl_reg__REMOVED)

static counter_reset_BACKUP_STRUCT  counter_reset_backup = {0u};

    
/*******************************************************************************
* Function Name: counter_reset_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void counter_reset_SaveConfig(void) 
{
    counter_reset_backup.controlState = counter_reset_Control;
}


/*******************************************************************************
* Function Name: counter_reset_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void counter_reset_RestoreConfig(void) 
{
     counter_reset_Control = counter_reset_backup.controlState;
}


/*******************************************************************************
* Function Name: counter_reset_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void counter_reset_Sleep(void) 
{
    counter_reset_SaveConfig();
}


/*******************************************************************************
* Function Name: counter_reset_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void counter_reset_Wakeup(void)  
{
    counter_reset_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */

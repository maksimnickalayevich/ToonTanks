// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHPWidget.h"

#include "Components/ProgressBar.h"

void UEnemyHPWidget::SetHPPercent(float NewHealthPercent)
{
	this->HPBar->SetPercent(NewHealthPercent);
}

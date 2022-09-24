// Fill out your copyright notice in the Description page of Project Settings.


#include "Chat/UserChatWidget.h"
#include "Chat/ChatMessageWidget.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"

//void UUserChatWidget::AddMessage(FName sender, FText message)
//{
//	UChatMessageWidget* added_message = CreateWidget<UChatMessageWidget>(GetOwningPlayer(), UChatMessageWidget::StaticClass());
//	added_message->sender = sender;
//	added_message->message = message;
//
//	messages->AddChild(added_message);
//}
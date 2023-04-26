#pragma once
#include "transaction.h"

// объявление интерфейса
class ITransactionFactory
{
public:
    virtual Transaction* CreateTransaction() = 0;
};

// класс для покупки
class PurchaseTransactionFactory : public ITransactionFactory
{
public:
    Transaction* CreateTransaction() override
    {
        return new PurchaseTransaction();
    }
};

// класс для перевода денег
class TransferTransactionFactory : public ITransactionFactory
{
public: 
    Transaction* CreateTransaction()
    {
        return new TransferTransaction();
    }
};
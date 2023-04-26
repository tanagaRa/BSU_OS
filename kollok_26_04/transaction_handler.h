#pragma once
#include "transaction.h"
#include "transaction_factory.h"

class ITransactionProcessor
{
public:
    virtual void ProcessTransaction(Transaction* transaction) = 0;
};

class PurchaseTransactionProcessor : public ITransactionProcessor
{
public:
    void ProcessTransaction(Transaction* transaction)
    {
        PurchaseTransaction* purchaseTransaction = dynamic_cast<PurchaseTransaction*>(transaction);

        if (purchaseTransaction != nullptr)
        {
            // списываем деньги с банковского счета пользователя
        }
    }
};

class TransferTransactionProcessor : public ITransactionProcessor
{
public: 
    void ProcessTransaction(Transaction transaction)
    {
        TransferTransaction* transferTransaction = dynamic_cast<TransferTransaction*>(transaction);

        if (transferTransaction != nullptr)
        {
            // зачисляем деньги на другой счет
        }
    }
};


class TransactionHandler
{
public:
    TransactionHandler(ITransactionFactory* purchaseTransactionFactory,
                        ITransactionProcessor* purchaseTransactionProcessor,
                        ITransactionFactory* transferTransactionFactory,
                        ITransactionProcessor* transferTransactionProcessor)
        : purchaseTransactionFactory(purchaseTransactionFactory),
          purchaseTransactionProcessor(purchaseTransactionProcessor),
          transferTransactionFactory(transferTransactionFactory),
          transferTransactionProcessor(transferTransactionProcessor)
    {}

    void HandleTransaction(TransactionType transactionType)
    {
        Transaction* transaction = nullptr;

        switch (transactionType)
        {
            case TransactionType::Purchase:
                transaction = purchaseTransactionFactory->CreateTransaction();
                purchaseTransactionProcessor->ProcessTransaction(transaction);
                break;
            case TransactionType::Transfer:
                transaction = transferTransactionFactory->CreateTransaction();
                transferTransactionProcessor->ProcessTransaction(transaction);
                break;
            default:
                throw std::invalid_argument("Invalid transaction type");
        }
    }

private:
    ITransactionFactory* purchaseTransactionFactory;
    ITransactionProcessor* purchaseTransactionProcessor;
    ITransactionFactory* transferTransactionFactory;
    ITransactionProcessor* transferTransactionProcessor;
};
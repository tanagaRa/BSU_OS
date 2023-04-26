#include <gtest/gtest.h>
#include "transaction_handler.h"
//#include "MockTransactionFactory.h"
//#include "MockTransactionProcessor.h"

// Тесты для PurchaseTransactionFactory
TEST(PurchaseTransactionFactoryTest, CreateTransaction_ReturnsPurchaseTransaction)
{
    // Arrange
    PurchaseTransactionFactory purchaseTransactionFactory;

    // Act
    Transaction* transaction = purchaseTransactionFactory.CreateTransaction();

    // Assert
    EXPECT_TRUE(dynamic_cast<PurchaseTransaction*>(transaction) != nullptr);
    delete transaction;
}

// Тесты для PurchaseTransactionProcessor
TEST(PurchaseTransactionProcessorTest, ProcessTransaction_UpdatesUserAccount)
{
    // Arrange
    MockUserAccount userAccount;
    PurchaseTransactionProcessor purchaseTransactionProcessor(&userAccount);
    PurchaseTransaction purchaseTransaction;
    ON_CALL(userAccount, DeductFromBalance(_)).WillByDefault(Return(true));

    // Act
    purchaseTransactionProcessor.ProcessTransaction(&purchaseTransaction);

    // Assert
    EXPECT_TRUE(userAccount.VerifyAndClear());
}

// Тесты для TransactionHandler
TEST(TransactionHandlerTest, HandleTransaction_WithPurchaseTransaction_CreatesAndProcessesPurchaseTransaction)
{
    // Arrange
    MockPurchaseTransactionFactory purchaseTransactionFactory;
    MockPurchaseTransactionProcessor purchaseTransactionProcessor;
    MockTransferTransactionFactory transferTransactionFactory;
    MockTransferTransactionProcessor transferTransactionProcessor;
    TransactionHandler transactionHandler(&purchaseTransactionFactory, &purchaseTransactionProcessor,
                                           &transferTransactionFactory, &transferTransactionProcessor);
    ON_CALL(purchaseTransactionFactory, CreateTransaction()).WillByDefault(Return(new PurchaseTransaction()));
    EXPECT_CALL(purchaseTransactionProcessor, ProcessTransaction(_)).Times(1);

    // Act
    transactionHandler.HandleTransaction(TransactionType::Purchase);
}

TEST(TransactionHandlerTest, HandleTransaction_WithInvalidTransactionType_ThrowsArgumentException)
{
    // Arrange
    MockPurchaseTransactionFactory purchaseTransactionFactory;
    MockPurchaseTransactionProcessor purchaseTransactionProcessor;
    MockTransferTransactionFactory transferTransactionFactory;
    MockTransferTransactionProcessor transferTransactionProcessor;
    TransactionHandler transactionHandler(&purchaseTransactionFactory, &purchaseTransactionProcessor,
                                           &transferTransactionFactory, &transferTransactionProcessor);

    // Act & Assert
    EXPECT_THROW(transactionHandler.HandleTransaction(static_cast<TransactionType>(-1)), std::invalid_argument);
}
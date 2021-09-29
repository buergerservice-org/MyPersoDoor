g++ -c workflowClient.cpp
g++ -o workflowClient workflowClient.o workflowLibrary.a -lssl -lcrypto -lpthread

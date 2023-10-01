import numpy as np
import matplotlib.pyplot as plt
import warnings
from sklearn.preprocessing import OneHotEncoder,OrdinalEncoder

#math functions
def softmax(x):
    x = x-np.max(x,0)[None,:]
    x = np.exp(x)
    return x/np.sum(x,0)[None,:]

def sigmoid(x):
    return 1/(np.exp(-x)+1)

def dsigmoid(x):
    return x * (1 - x)

def hypertan(x):
    return 2 * sigmoid(x) - 1

def dhypertan(x):
    return 1 - (x ** 2)

def relu(x):
    return np.maximum(x,np.zeros(x.shape))

def drelu(x):
    x[x != 0] = 1
    return x

#gradient descent implementation
#for normal gradient descent set model.batch_size = 1
def stochastic_gradient_descent(model):

    N = model.x.shape[0]
    M = int(N * model.batch_size)
    if M < 1:
        M = 1
        warnings.warn('Warning, batch_size value results in less than one instance per batch. Automatically converting to one instance per batch.')
    c = 0

    if(model.graph_progress):
        accuracies = []

    while(c < model.epochs):

        c += 1
        i = 1

        while(M*(i-1) < N):
            L = M*(i-1)
            R = np.min([M*i,N])
            batch_x = model.x[L:R]
            batch_y = model.y[L:R]
            dw,db = model.gradient(batch_x,batch_y)
            for j in range(1,len(model.w_)):
                model.w_[j] = model.w_[j] - model.learning_rate * dw[j]
                model.b_[j] = model.b_[j] - model.learning_rate * db[j]
            i += 1
        
        if(model.graph_progress):
            score = scorer(model,model.x_test,model.y_test)
            print("Epoch",c,". Accuracy:",score)
            accuracies.append(score)

    if(model.graph_progress):
        plt.plot(np.arange(1,model.epochs+1),accuracies)
        plt.show()

def scorer(estimator,X,y):
    return np.sum(y == estimator.predict(X))/y.shape[0]


#neural network implementation
class Neural_net():

    def __init__(self,shape=[100,100],activation='relu',learning_rate=0.01,epochs=50,batch_size=0.2,reg=0.1):
        
        self.shape = shape
        self.activation = activation
        self.learning_rate = learning_rate
        self.epochs = epochs
        self.batch_size = batch_size
        self.reg = reg

        if self.activation == 'relu':
            self.f = relu
            self.df = drelu
        if self.activation == 'sigmoid':
            self.f = sigmoid
            self.df = dsigmoid
        if self.activation == 'hypertan':
            self.f = hypertan
            self.df = dhypertan

    def fit(self,x,y,x_test=None,y_test=None):

        self.encoder_ordinal = OrdinalEncoder()

        #storing the training data
        self.x=x
        self.encoder_1hot = OneHotEncoder(sparse_output=False)
        self.y=self.encoder_1hot.fit_transform(self.encoder_ordinal.fit_transform(y[:,None]))

        #storing optional test data for evaluation during training
        self.graph_progress = x_test is not None and y_test is not None
        self.x_test = x_test
        self.y_test = y_test

        #initializing the weights 
        D = self.x.shape[1]
        C = self.y.shape[1]

        self.w_ = []
        self.w_.append(np.zeros(0))
        self.b_ = []
        self.b_.append(np.zeros(0))

        self.shape.insert(0,D)
        self.shape.append(C)
        std = np.sqrt(2/D)
        
        for i in range(0,len(self.shape)-1):
            self.w_.append(np.random.normal(0,0.3,size=[self.shape[i+1],self.shape[i]]))
            self.b_.append(np.random.uniform(0,0.1,size=[self.shape[i+1],1]))
        self.shape.pop()
        self.shape.pop(0)

        #optimizing the weights
        stochastic_gradient_descent(self)

    def predict(self,x):
        
        l = len(self.w_)
        a = x.T
        i = 1
        while i < l-1:
            a = self.f(self.w_[i] @ a + self.b_[i])
            i += 1
        a = softmax(self.w_[i] @ a + self.b_[i])
            
        return self.encoder_ordinal.inverse_transform(np.argmax(a.T,axis=1)[:,None]).flatten()
    
#returns the gradient, which depends on four inputs w,b (current location in the parameter
#space) and x,y (needed to evaluate cost of current location, and thus the gradient also depends on these)
#as well as two hyperparameters, activation/dactivation, which determine the form of the neural net

#w,b is a list of arrays, where w[i] and b[i] are the weights and biases going into layer i
#thus w[0] is an empty array, since no weights and biases go into layer 0 (the input layer)

    def gradient(self,x=None,y=None):

        if x is None and y is None:
            x = self.x
            y = self.y

        N,_ = x.shape
        l = len(self.w_)
        a = []
        da = []
        dw = []
        db = []

        a.append(x.T)
        i = 1
        while i < l-1:
            a.append(self.f(self.w_[i] @ a[i-1] + self.b_[i]))
            i += 1
        a.append(softmax(self.w_[i] @ a[i-1] + self.b_[i]))

        da.append(np.zeros(0))
        i = 1
        while i < l-1:
            da.append(self.df(a[i]))
            i += 1

        i = l-1
        g = a[i] - y.T
        dw.append(g @ a[i-1].T / N)
        db.append(g @ np.ones(shape=[N,1]) / N)
        while i > 1:
            g = self.w_[i].T @ g * da[i-1]
            dw.append(g @ a[i-2].T / N)
            db.append(g @ np.ones(shape=[N,1]) / N)
            i -= 1

        dw.append(np.zeros(0))
        db.append(np.zeros(0))
        dw.reverse()
        db.reverse()

        for i in range(1,l):
            dw[i] += (self.reg / N) * self.w_[i]

        return dw, db
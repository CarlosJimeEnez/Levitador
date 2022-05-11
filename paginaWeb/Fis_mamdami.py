from tracemalloc import start, stop
import numpy as np
class Fis: 
    def __init__(self, **kw):
        self.name = kw['name']
        self.rango = kw['rango']
        self.func_memb = kw['func_membr']
        self.func_value = np.zeros(np.size(self.rango))
        self.func_dicc_values = {}
        self.belonging_value_return = []

    def build_function(self):
        '''Retorna un diccionario con los valores de cada grafica en distintas keys''' 
        for funcion in self.func_memb: 
            #Gaussiana:
            if (funcion['tipo'] == 'exp'): 
                k = funcion['k']
                m = funcion['m']
                rango = self.rango
                value = self.func_value
                for i in np.arange(start=0, stop=np.size(self.rango)):
                    value[i] = np.exp(-k*(rango[i]-m)**2)

                value = np.copy(value)
                new_value = {funcion['nombre']:value}
                self.func_dicc_values.update(new_value)
            if (funcion['tipo'] == 'gauss'): 
                b = funcion['b'] 
                c = funcion['c']
                rango = self.rango 
                value =self.func_value
                for i in np.arange(start=0, stop=np.size(self.rango)): 
                    value[i] = 2*(np.exp(-(rango[i] - b)**2))/(2*c**2)
                    
                value = np.copy(value)
                new_value = {funcion['nombre']:value}
                self.func_dicc_values.update(new_value)

        return self.func_dicc_values
    
    def belonging_value(self, start, input_value):
        '''Retorna el valor de pertenencia a cada funcion de membresia de e el valor de entrada dado:'''
        new_range = np.arange(start = start, stop=input_value, step=0.001)
        belonging_value = np.zeros(np.size(new_range))
        for function in self.func_memb: 
            #Gaussiana: 
            if function['tipo'] == 'exp':
                k = function['k']
                m = function['m'] 
                for i in np.arange(start= 0, stop=np.size(new_range)): 
                    belonging_value[i] = np.exp(-k*(new_range[i]-m)**2)
                
                belonging_value = np.copy(belonging_value)
                self.belonging_value_return.append(belonging_value[-1])  

        return self.belonging_value_return        

    def belonging_value2(self, start, payload, const):
        items_tuple = self.func_dicc_values.items()

        for item in items_tuple: 
            print(item[0])
            fuzzy_val = item[1][ int(const*(payload - (start)))] 
            self.belonging_value_return.append(fuzzy_val)
        
        return self.belonging_value_return

    def clear_belonging_value(self): 
        self.belonging_value_return = []
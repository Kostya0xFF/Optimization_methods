import numpy as np
import matplotlib.pyplot as plt

def f(x):
    return np.cos(x - 1) + np.abs(x)

x = np.linspace(-2, 2, 500)
y = f(x)

plt.figure(figsize=(8, 6))
plt.plot(x, y, linewidth=1, label='f(x) = cos(x-1)+|x|')
plt.xlabel('x')
plt.ylabel('f(x)')
plt.title('График функции f(x)')
plt.grid(True, linestyle='--', alpha=0.5)
plt.legend()
plt.show()
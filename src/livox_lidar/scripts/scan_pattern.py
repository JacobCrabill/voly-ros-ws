import numpy as np
import matplotlib.pyplot as plt

if __name__=='__main__':
  for i in range(5, 15):
    print(i)
    delta_theta = 0.001
    thetas = np.zeros(0)
    rs = np.zeros(0)

    n = float(i)
    d = 2 * np.pi
    alpha = (1.0 + n**2) / n**3
    phase_shift = alpha * d

    for t in range(0, 200):
      theta = np.arange(0, 2 * np.pi, delta_theta)
      r = 0.5 + 0.5 * np.cos(n * theta + phase_shift * t % (2 * np.pi))
      thetas = np.append(thetas, theta)
      rs = np.append(rs, r)

    plt.polar(thetas, rs)
    plt.show()
import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

plt.rcParams['text.usetex'] = True
plt.rcParams["font.family"] = "Times New Roman"
bwidth = 2.0
font_size  = 15
label_size = 15
marker_size= 3
line_width = 1.2
legend_size= 13

#--------------------------------------------------------------------------#
# data
#--------------------------------------------------------------------------#

saveplot  = 0
case_path = 'build/examples/'
case_name = 'shock_tube'
# case_name = 'linear_adv'

parent_dir = os.path.join(os.path.dirname(__file__), os.pardir)
file_path = os.path.join(parent_dir, case_path + case_name + '_10.csv')

data = pd.read_csv(file_path)

var_name = 'p'
x = data['x']
var = data[var_name]

# analytical solution
sol  = np.loadtxt('vis/exact.csv')
x_ex = sol[0,:]
var_ex = sol[1,:]
if var_name == 'rho': var_ex = sol[1,:]
if var_name == 'u': var_ex = sol[2,:]
if var_name == 'p': var_ex = sol[3,:]
#--------------------------------------------------------------------------#
# plot
#--------------------------------------------------------------------------#
fig, ax = plt.subplots()
ax.plot(x, var, 'ko', markersize = marker_size, label='numerical')
ax.plot(x_ex, var_ex, 'k-', linewidth = line_width, label='analytical')

# ax.plot(t, n_cells, 'o')
ax.spines['top'].set_linewidth(bwidth)
ax.spines['bottom'].set_linewidth(bwidth)
ax.spines['left'].set_linewidth(bwidth)
ax.spines['right'].set_linewidth(bwidth)
ax.axes.xaxis.set_ticks_position('both')
ax.tick_params(axis='x', direction='in', labelsize=label_size)
ax.set_xlabel(r'$x$', fontsize=font_size)
# ax.set_xlim(-2.0*pi,2.0*pi)
# ax.set_xticks([-2.0*pi,-pi,0,pi,2.0*pi])
# ax.set_xticklabels([r'$-2\pi$', r'$-\pi$', 0, r'$\pi$', r'$2\pi$'], size=label_size)
ax.axes.yaxis.set_ticks_position('both')
ax.tick_params(axis='y', direction='in', labelsize=label_size)
ax.set_ylabel(r'$p$', fontsize=font_size)
ax.legend(loc='best',frameon=False,fontsize=legend_size)

plt.show()
fig.savefig('vis/shock_tube_'+ var_name + '.pdf', bbox_inches='tight')



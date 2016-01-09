
![Transmission line elements](https://cloud.githubusercontent.com/assets/11730626/12213249/2f375514-b6c9-11e5-8e5b-dff241defc66.png)


![Transmission line states](https://cloud.githubusercontent.com/assets/11730626/12213251/337a6062-b6c9-11e5-879a-1aa1ace4c04a.png)


![equation flux charge](https://cloud.githubusercontent.com/assets/11730626/12213248/28a1d9fe-b6c9-11e5-96d3-4c0d26ecf121.png)

<!--
{dq_n\over dt}=\frac1L \varphi_n-\frac1L \varphi_{n+1}-\frac GC q_n~~~~~~~~~~~(n=1,...,N-1)\\
~\\
~\\
{dq_N\over dt}=\frac1L \varphi_N-\frac GC q_N
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~\\
~\\
~\\
~\\
{d\varphi_1\over dt}=-\frac1C q_1-\frac RL \varphi_1+v_{in}(t)~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\
~\\
~\\
{d\varphi_n\over dt}=\frac1C q_{n-1}-\frac1C q_n-\frac RL \varphi_n~~~~~~~~~~~~~~~~~~(n=2,...,N)\\
 -->
<br />
<br />
<br />
<br />

**State equations**

![equation states](https://cloud.githubusercontent.com/assets/11730626/12213387/4dc526ee-b6cb-11e5-99df-a108bd9d7a07.png)

<!--
{dx_n\over dt}=-\frac GC x_n+\frac1L x_{N+n}-\frac1L x_{N+n+1}~~~~~~~~~~~(n=1,...,N-1)\\
~\\
~\\
{dx_N\over dt}=-\frac GC x_N+\frac1L x_{2N}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~\\
~\\
~\\
~\\
{d x_{N+1}\over dt}=-\frac1C x_1-\frac RL x_{N+1}+v_{in}(t)~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\
~\\
~\\
{dx_{N+n}\over dt}=\frac1C x_{n-1}-\frac1C x_n-\frac RL x_{N+n}~~~~~~~~~~~~~~~~~~(n=2,...,N)\\
 -->
<br />
<br />
<br />
<br />


**Results**

The input voltage at the line entrance:
![The input voltage at the line entrance](https://cloud.githubusercontent.com/assets/11730626/12201957/3e32148a-b67b-11e5-90f4-6ca9f021f3a9.png)

The output voltage at the line terminal:
![The output voltage at the line terminal](https://cloud.githubusercontent.com/assets/11730626/12201933/1be8200e-b67b-11e5-84cb-8a335f55cb51.png)

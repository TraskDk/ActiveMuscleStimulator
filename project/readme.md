# Movement Translation Math

## 1. Introduction

The purpose of Movement Translation math is enabling the device to translate a small
history of movement data into progress and movement speed of a previously defined
movement.

### 1.1 Terminology

In the following we will use _p_ to denote a number between 0 and 1, which describes
how far we have come in the movement. We would typically enable muscle activations at
particular values of _p_, and for symmetric activation we the two sides would typically
have a difference in activation times of 0.5.

In addition, we will use _s_ to denote the speed by which the movement is executed
(a floating point number in the unit of movements per minute). A walk might give
a value of _s_=45 movements/minute, while an aggressive sprint might give values of 
_s_=125 movements/minute.

We define a movement spaning up until the movement pattern is repeated.
In particular, this means that movements per minute is half steps per minute.

### 1.2 Problem Definition

Given O, a sequence of observation (t_i, x_i) for i=1...N, we would like to know the most
probable value of _p_ along with the associated likelihood. If the likelihood is low, 
it appears the user is not performing the movement at all, and we should avoid doing 
any muscle stimulations. On the other hand, if the likelihood is high, we now know 
exactly the progress in the movement, and we know what muscles should be stimulated.

In order to reach this ability, we shall assume to have demonstrated the movements
with associated information about when in the movement the muscle stimulations should
be happening. What we do is then to create a probabilistic model of P(x|p,s). For 
simplicity, we would assume to observe a constant _s_ for a short observation period.

We would expect being able to express P(p|O) = \sum_s P(p,s|O). Further, we notice that
P(p,s|O) \propto P(O|p,s)P(p,s). For simplicity, we would assume that all progresses and
speeds are equaly likely, so all we need to think about is P(O|p,s), which can be expressed
as \prod_i P(x_i|p,s). So we are back to the expression from the previous paragraph.

In order to compensate for issues rearding changes in movement, such a sudden change in
pace, we would like to change the product \prod_i P(x_i|p,s), such that it is changed into 
an exponential filter, that values recent observations higher that older observations.

## 2. Architecture

### 2.1 Real-time Inference

In order implement the proposed probabilistic algorithm as quickly as possible whenever we
are given a new observation _x_\__N_, it is clear that most of the computation we did when
receiveing the observations _x_\_1,...,_x_\_\{_N_-1\} must be reusable for performing
inference about _x_\_1,...,_x_\__N_.

We achieve this by introducing a class responsible for handling P(O|p,s) for a given s. 
For a fixed number of progress values (say 100 different evenly distributed values), 
objects of this class holds P(O|p,s). In other words, for 100 different values of p,
we have stored the value of P(O|p,s). When adding an additional observation to O, we
can reuse the existing values, but we noice that the values of p have changed in
accordance with _s_ and the time passed since the last observation 
_d_ = _t_\__N_ - _t_\_\{_N_-1\}. If before we were at d', we would now
expect to be at p = frac(p' + d * s/60).

We notice that we could look at I=100*(d*s/60), where 100 was the number of progress
values we are modeling, and simply rotate the values this number of places. However,
if we are receiving 100 updates per second, we notice that for a walk (s=30), the number
of indexes by which to perform the rotation is I=round(100*(0.01*s/60))=round(0.5).
This clearly will not work, as the rotation will be performed at twice the appropriate
speed.

In order to make this usable, we would need to work with an accumulated rounding error.
We can handle this by introducing remainder from the previous calculations r_\{i-1\},
and let I'_i=100*(d_i*s/60 + r_\{i-1\}). We then redefine I=floor(I'_i) as the actual
index to use, and we let r_i=frac(I'_i).

What is does is that it enables us to reuse most of the the computations we have done
for all the previous observations.

### 2.2 Learning

Let's now turn our attention to our ability to construct a function that tells
us P(x|p,s). Assuming that a specic muscle would stimulated exactly once during each
movement, we could choose to let the movement start when that muscle stimulation
starts. We could maybe execute the movement at different speeds, which may or may not
change the characteristics of the movement.

A simplistic model assumes that movement looks approximately the same independently 
of the speed at which it is executed. We shall construct this as a baseline model.

In addition to the baseline model, it would be relevant to consider two extensions:

  1) Enabling a model to interpolate between tought movement patterns at different
     speeds.
  2) Enabling the model to consist of completely different movements, each with 
     their own range of execution speeds and muscle activation patterns.

We could think of 1) as the ability to know running style changes when going from
rogging all the way to sprinting.

We could think of 2) as also know about how to walk, as a fast walk could be
performed at the same speed as jogging (in terms of movements per minute), 
but the movements would look very different. Instead of modeling the average 
of the two patterns, it would be better to have a model for each. As part
of the inference, we would need to know in addition to d what the model is,
since different models might have different timings for muscle activations.

But the starting point is the baseline model (implemented via an abstract class,
so we can try out various enhancements in the future).

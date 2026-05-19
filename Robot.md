
March 30: Started robot development

April 6: Calibration code + basic line follower + robot assembly

April 11: Robot assembly + line follower test

April 13: Finished line follower, working

April 18: Green tape detection

April 20: Improvements on green tape handling


Formulas for basic movements:

Degrees = 360 x Distance / π x Diameter(wheel)

Perimeter(robot) = Diameter(robot) x π
 
90° = L = Perimeter(robot)/4  -- Note that the result is in cm, so its necessary to convert on degrees

Calibration: white (max value)
			black (min value)

Scale convertion:
i1 and i2: ideal values, on same scale
i1 = 100(r1-black1)/(white1-black1)
(the same goes for i2, with white2 and black2)
error = i1-i2



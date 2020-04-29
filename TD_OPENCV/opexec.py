import cv2
import imutils
import numpy as np

# define range of color in HSV
lower = np.array([15,50,50])
upper = np.array([35,255,255])

def onPostCook(changeOp):
	frame = changeOp.numpyArray()
	bgr = cv2.cvtColor(frame, cv2.COLOR_RGBA2BGR)
	bgr = bgr * 255.0
	bgr = bgr.astype(np.uint8)
	blurred = cv2.GaussianBlur(bgr, (11, 11), 0)
	hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)
	# construct a mask for the color "green", then perform
	# a series of dilations and erosions to remove any small
	# blobs left in the mask
	mask = cv2.inRange(hsv, lower, upper)
	mask = cv2.erode(mask, None, iterations=2)
	mask = cv2.dilate(mask, None, iterations=2)
	# find contours in the mask and initialize the current
	# (x, y) center of the ball
	cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
		cv2.CHAIN_APPROX_SIMPLE)
	cnts = imutils.grab_contours(cnts)
	center = None
	# only proceed if at least one contour was found
	if len(cnts) > 0:
		# find the largest contour in the mask, then use
		# it to compute the minimum enclosing circle and
		# centroid
		c = max(cnts, key=cv2.contourArea)
		((x, y), radius) = cv2.minEnclosingCircle(c)
		# only proceed if the radius meets a minimum size
		if radius > 10:
			op('position').par.value0 = x
			op('position').par.value1 = y
			op('position').par.value2 = radius

	return
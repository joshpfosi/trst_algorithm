#!/usr/bin/python2
import sys
import math
import re
from kivy.graphics import *
from kivy.app import App
from kivy.animation import Animation
from kivy.uix.relativelayout import RelativeLayout
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.gridlayout import GridLayout
from kivy.uix.anchorlayout import AnchorLayout
from kivy.clock import Clock
from kivy.uix.widget import Widget
from random import randint, choice
from time import sleep
from threading import Thread
from collections import deque
from kivy.properties import StringProperty, BooleanProperty, NumericProperty
from kivy.config import Config
from kivy.uix.button import Button
from functools import partial

# This file defines the logic for the gui
# The basic idea is that a static map image of large enough size is used
# with known reference latitudes / longitudes allowing for a lat / lon to 
# pixel conversion

# The code reads from stdin (stdinRead) expecting alternating lines of 
# algorithm
# human
# algorithm
# human...
# data, which it loads into a queue, representing each line as a dictionary
# with keys that give meaning to the floats (see stdinRead)

# Using Clock, a scheduled callback is called every BAUD_RATE seconds which
# updates the screen with the next line of algor/human data - this is flexible
# for when we actually implement the algorithm and need to test it

# The rest is pretty much just logic to make that happen and widget declarations
# NOTE: In Kivy, you "need" a Python class to have a .kv representation of it
#       A class which does nothing but visually appears uses the keyword "pass"
#       which is Pythonic for "Nothing to see here, define and move on"


# Let me know what you think, but I feel the best work flow will be to 
# attack this TODO list and add items as time goes on - comment like its 40!
# Test after each change! Don't break it!

# NOTE: This file expects stdin! Give it:

# ! kivy __main__.py < data.txt

# The data.txt file is generated by data_gen.py - feel free to add new data
# as widgets which display that data are implemented

# TODO (in no particular order)
# * Add latitude and longitude to the GUI on mouseover (i.e. the latlon of 
#    the mouse)
# * Implement a polar diagram interface (not super pressing)
# * Add a velocity widget for algorithm "side"
# * Any cosmetic changes you like!
# * Add wind speed, apparent wind direction, and apparent wind speed test data
#    to data_gen.py
# * Add Pause button to pause callback

maxLon = -105.1621          # top right longitude
maxLat = 40.0868            # top right latitude
minLon = -105.2898          # bottom left longitude
minLat = 40.001             # bottom left latitude

# image dimensions in pixels
MAP_WIDTH = 1049
MAP_HEIGHT = 700

# width of side nav bars
SIDEBARS = 200
NAVBAR = 50

# fake baud rate
BAUD_RATE = 1

def latToY(targetLat):
    """
    Returns pixel representation of latitude using given reference coords
    """
    return ((targetLat - minLat) / (maxLat - minLat)) * (MAP_HEIGHT - 1) + NAVBAR

def lonToX(targetLon):
    """
    Returns pixel representation of longitude using given reference coords
    """
    return ((targetLon - minLon) / (maxLon - minLon)) * (MAP_WIDTH - 1) + SIDEBARS
    
class stdinRead(Thread):
    #set as a thread to allow for lags in stdin or no stdin at all
    def __init__(self, *largs, **kwargs):
        super(stdinRead, self).__init__(*largs, **kwargs)
        self.daemon = True
        self.queue = deque()
        self.quit = False
        self.index = 0

    def parse_data(self, data):
        """
        Parses semicolon dilimited list of floats into dictionary 
            w/ appropriate key values
        """
            
        data = data.split(';')
        return {'Lat' : (float(data[1])), 'Lon' : (float(data[0])), 
                'RudPos' : (float(data[2])), 'Wind Dir' : data[3], 'Wind Speed' : data[4]}

    def run(self):
        """
        Populates queue w/ dictionaries w/ each line of input
        """
        q = self.queue
        while not self.quit:
            data = sys.stdin.readline().strip()
            if data is None or len(data)==0:
                sleep(1)
                continue
            q.appendleft(self.parse_data(data))
            sleep(.01)

    def pop(self):
        return self.queue.pop()

class Marker(Widget):        
    pass

class MarkerGreen(Widget):
    pass

class Map(Widget):
    pass

class Updater(Widget):
    def pullData(self, map, envdata, rudderpos, dt):
        """
        Updates all gui elements with fresh data from stdin queue
        """
        try:
            # The repetition of code here is sad but subtly difficult to abstract
            data_algor = self.stdin.pop()
            data_human = self.stdin.pop()

            # update algorithm lat/lon
            x = lonToX(data_algor['Lon'])
            y = latToY(data_algor['Lat'])
            print 'Lon: %f' % x
            print 'Lat: %f' % y
            marker = Marker(pos = (x - 5, y - 5))
            marker.layout = map
            map.add_widget(marker)

            # update human lat/lon
            x = lonToX(data_human['Lon'])
            y = latToY(data_human['Lat'])
            print 'Lon: %f' % x
            print 'Lat: %f' % y
            markerGreen = MarkerGreen(pos = (x - 5, y - 5))
            markerGreen.layout = map
            map.add_widget(markerGreen)


            r = 75
            rudderpos.canvas.clear()
            # update algorithm rudder pos
            theta = (math.pi * data_algor['RudPos']) / 180
            print theta
            print r * math.cos(theta)
            print r * math.sin(theta)
            with rudderpos.canvas:
                Color(1, 0, 0)
                Line(points = (100, 100, 100 + r * math.cos(theta), 100 + r * math.sin(theta)))

            # update human rudder pos 
            theta = (math.pi * data_human['RudPos']) / 180
            print theta
            print r * math.cos(theta)
            print r * math.sin(theta)
            with rudderpos.canvas:
                Color(0, 1, 0)
                Line(points = (100, 100, 100 + r * math.cos(theta), 100 + r * math.sin(theta)))

            envdata.wind_direction = data_algor['Wind Dir']
            envdata.wind_speed = data_algor['Wind Speed']
            envdata.app_wind_direction = data_algor['Wind Dir']
            envdata.app_wind_speed = data_algor['Wind Dir']

        except Exception as e:
            print(e)
            return

class NavBar(GridLayout):
    def build(self):
        self.add_widget(self.envdata)
        self.add_widget(self.map)

class EnvData(GridLayout):
    # Public data basically that can be updated
    # Starts off null
    wind_direction = StringProperty('')
    wind_speed = StringProperty('')
    app_wind_direction = StringProperty('')
    app_wind_speed = StringProperty('')

class AlgorData(GridLayout):
    def build(self):
        self.add_widget(self.rp)

class RudderPosition(Widget):
    def build(self):
        pass

class gui(App):
    def build_config(self, config):
        # set window height and width - give space for sidebars and map dimensions
        Config.set('graphics','width', MAP_WIDTH + SIDEBARS)
        Config.set('graphics','height', MAP_HEIGHT + NAVBAR)

    def build(self):
        # declare outter layout to hold inner elements
        layout = GridLayout(cols = 2, cols_minimum = {0 : SIDEBARS, 1 : MAP_WIDTH})

        # holds algorithm rudder / sail angles 
        algordata = AlgorData(rows = 3)
        algordata.rp = RudderPosition()
        algordata.build()

        # holds environmental data
        navbar = NavBar(rows = 2, rows_minimum = {0 : NAVBAR, 1: MAP_HEIGHT})
        navbar.envdata = EnvData(cols = 4)
        navbar.map = Map()
        navbar.build()

        layout.add_widget(algordata)
        layout.add_widget(navbar)

        updater = Updater()
        updater.stdin = stdinRead()
        updater.stdin.start()

        Clock.schedule_interval(partial(updater.pullData, navbar.map, 
            navbar.envdata, algordata.rp), BAUD_RATE)

        return layout

if __name__ == '__main__':
    gui().run()

# Streamray
A small CPU raytracer written during livestreams on twitch at https://www.twitch.tv/bellaedris

# Results this far

## Stream 2
Added a plane primitive, a scene representation and basic point lights.
![stream2.png](Media/stream2.png)

## Stream 1
Mostly setup and configuration work. Includes glm lib and gtest for automated tests.
Very basic ray tracing setup: we send rays through a camera that has a fixed resolution and a viewport in the world. 
The scene is an array of simple sphere objects that we check with each ray sent through our camera. The colors are hard
coded.
![stream1.png](Media/stream1.png)
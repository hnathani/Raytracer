CC=cl
CFLAGS=-D _USE_MATH_DEFINES /EHsc
SOURCES=main.cpp SceneObjects/Camera.cpp SceneObjects/Ray.cpp SceneObjects/Scene.cpp Objects/Object.cpp Objects/Sphere.cpp Lights/Light.cpp Lights/PointLight.cpp Lights/ParallelLight.cpp Objects/Mesh.cpp Objects/Texture.cpp Lights/SpotLight.cpp 
EXECUTABLE=raytracer.exe

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): 
	$(CC) $(CFLAGS) $(SOURCES) /Fe$@

clean:
	del *.obj
	del $(EXECUTABLE)

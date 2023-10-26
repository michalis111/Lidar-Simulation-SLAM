#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Image.hpp> 
#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>

// Define constants
const int windowWidth = 512;
const int windowHeight = 512;
const float PI = 3.14159265f; // Define PI constant for angle calculations
int angleResolution;
int range;

sf::Color wall(29,29, 27); // Assuming black pixel color is RGB(0, 0, 0)
sf::Image image;
// Create a class for the robot
class Robot {
public:
    sf::RectangleShape shape;
    sf::Vector2f position;


    Robot(float x, float y) {
        shape.setSize(sf::Vector2f(20, 20));
        shape.setFillColor(sf::Color::Red);
        position.x = x;
        position.y = y;

    }
    void rotate(float angle){
       //shape.setOrigin(position.x+shape.getSize().x, position.y-shape.getSize().y); // Set the origin to the center
 
    	shape.setRotation(shape.getRotation() + angle);
    }


    

    void move(float x, float y, const sf::Image& image) {
    	if(position.x<0){
		position.x =0;
	}
	if(position.x>windowWidth - shape.getSize().x){
		position.x = windowWidth - shape.getSize().x;
	}
	if(position.y<0){
		position.y =0;
	}
	if(position.y>windowHeight - shape.getSize().y){
		position.y = windowHeight - shape.getSize().y;
	}
	
	if(image.getPixel(position.x-1, position.y) == wall){
		position.x = position.x +0.1;
    		}
    	if(image.getPixel(position.x+shape.getSize().x, position.y) == wall){
		position.x = position.x -0.1;
    		}
    	if(image.getPixel(position.x, position.y-1) == wall){
		position.y = position.y +0.1;
    		}
    	if(image.getPixel(position.x, position.y+shape.getSize().y) == wall){
		position.y = position.y -1;
    		}
    	
	else{
	position.x += x;
        position.y += y;
        //std::cout<<position.x<<","<<position.y<<std::endl;
        }
        
  }


   
    void update() {
        shape.setPosition(position);

    }
}; 
    class Lidar {
    public:
    	sf::CircleShape lshape;
    	sf::Vector2f lposition;
    	

    	sf::Vector2f points;
    	
    	//float distances[10];
    	Lidar(float x, float y){
    		lshape.setRadius(5);
		lshape.setFillColor(sf::Color::Blue);
		lposition.x = x+5;
		lposition.y = y+5;
		
	   }
    void lmove(float x, float y, Robot* robot) {
    
	lposition.x = robot->position.x + lshape.getRadius();
        lposition.y = robot->position.y + lshape.getRadius();

        }
        
  
    	
    	void scan(int angle, int dist){

            float radians = angle * PI / 180.0f; // Convert angle to radians
            float x_offset = dist * std::cos(radians); // Calculate x offset
            float y_offset = dist * std::sin(radians); // Calculate y offset

            // Calculate the point at the specified angle and range
            
            points.x = lposition.x + x_offset;
            points.y = lposition.y + y_offset;
            std::cout<<"X: "<<points.x<<" "<<"Y: "<<points.y<<std::endl;
        
        }
        
    
 	
      void update() {
        lshape.setPosition(lposition);
    }
	    

   
    };
    

int main() {

    while (true) {
    std::cout << "Enter Lidar angle resolution (e.g., 10 degrees): ";
    std::cin >> angleResolution;
    std::cout << "Enter Lidar range (e.g., 5): ";
    std::cin >> range;

    if (angleResolution > 0 && angleResolution <= 360) {
        break; // Valid input, exit the loop
    }

    std::cout << "Invalid angle resolution. Please enter a value between 1 and 360." << std::endl;
}
    // Create a window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Robotics Simulation");
    sf::RenderWindow window2(sf::VideoMode(windowWidth, windowHeight), "Mapping");
    sf::RenderWindow window3(sf::VideoMode(windowWidth, windowHeight), "Localisation");
    
  

    if (!image.loadFromFile("/home/michalis111/Desktop/sim/floorplan.png")) {
    	std::cerr << "Failed to load floorplan image!" << std::endl;
    return 1;
}
        // Create a sprite to display the image
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite(texture);


    
    // Create a robot
    Lidar lidar(windowWidth / 2, windowHeight / 2);
    Robot robot(windowWidth / 2, windowHeight / 2);
    
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        // Handle font loading error
        return 1;
    }
    
    
    


 


       while (window.isOpen() && window2.isOpen() && window3.isOpen()) {
        sf::Event event1, event2, event3;
 

        while (window.pollEvent(event1)) {
            if (event1.type == sf::Event::Closed) {
                window.close();
            }
        }

        while (window2.pollEvent(event2)) {
            if (event2.type == sf::Event::Closed) {
                window2.close();
            }
        }
         while (window3.pollEvent(event3)) {
            if (event3.type == sf::Event::Closed) {
                window3.close();
            }
        }

        // Handle user input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            robot.move(0.0f, -0.1f,image);
            lidar.lmove(0.0f, -0.1f,&robot);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            robot.move(0.0f, 0.1f,image);
            lidar.lmove(0.0f, 0.1f,&robot);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            robot.move(-0.1f,0.0f,image);
            lidar.lmove(-0.1f,0.0f,&robot);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            robot.move(0.1f,0.0f,image);
            lidar.lmove(0.1f,0.0f,&robot);
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
       // Rotate the robot to the right (clockwise)
       robot.rotate(0.1f);
       }
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
       // Rotate the robot to the right (clockwise)
       robot.rotate(-0.1f);
       }


       
        // Update the robot's position and orientation
        robot.update();
        lidar.update();
        
       
        
	

        // Clear the window
        window.clear(sf::Color::White);
        window3.clear(sf::Color::Black);



      for (int angle = 0; angle <= 360; angle+=angleResolution) {
      	for(int dist=0; dist <= range; dist+=5){
	      	if(robot.position.y<dist)
		    	break;
        lidar.scan(angle,dist);
        
        if(image.getPixel(lidar.points.x, lidar.points.y) == wall){
            sf::Vector2f startPoint(lidar.lposition.x + lidar.lshape.getRadius(),lidar.lposition.y);
    sf::Vector2f endPoint(lidar.points.x+ lidar.lshape.getRadius(), lidar.points.y);
		    sf::VertexArray line(sf::Lines, 2);
		    line[0].position = startPoint;
		    line[0].color = sf::Color::Blue;
		    line[1].position = endPoint;
		    line[1].color = sf::Color::Blue;
		
		
		sf::RectangleShape pt; // Create a small circle to represent each point
		pt.setSize(sf::Vector2f(3, 3));
        pt.setFillColor(sf::Color::Red);
		//pt.setFillColor(sf::Color(0, 250, 0));
		pt.setPosition(lidar.points.x,lidar.points.y);
		
		
		
		
		window2.draw(pt);
		window.draw(line);
		break;
        }
        }
        }
        window.draw(sprite);
        // Draw the robot
        window.draw(robot.shape);
        window.draw(lidar.lshape);
        
        sf::RectangleShape loc; // Create a small circle to represent each point
	loc.setSize(sf::Vector2f(5, 5));
        loc.setFillColor(sf::Color::Blue);
	loc.setPosition(robot.position.x,robot.position.y);
        window3.draw(loc);
        
        
        
        
        sf::Text text;
	text.setFont(font);

   	std::string xCoord = "X: " + std::to_string(static_cast<float>(robot.position.x));
    	std::string yCoord = "Y: " + std::to_string(static_cast<float>(robot.position.y));

	text.setString(xCoord + " " + yCoord);
	text.setCharacterSize(16); // Set the font size
	text.setFillColor(sf::Color::White);
	text.setPosition(10, 10); // Set the text position
        window3.draw(text);
        
        
        

        // Display everything
        window.display();
        window2.display();
        window3.display();
    }

    return 0;
}


#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <SFML/Network.hpp>


//g++ -Wall -c "%f" -lBox2D -lserial -std=c++11

/*
 * Compiler et executer:
g++ -c sfmlPantograph.cpp;
g++ sfmlPantograph.o -o sfml-pantograph -lBox2D -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network; 
./sfml-pantograph
*/

// http://www.unixuser.org/lg/issue74/tougher.html
// http://www.sfml-dev.org/tutorials/2.1/network-socket.php

const double pi = 3.1415926535897;

float norm(sf::Vector2f v){
	return sqrt(v.x*v.x + v.y*v.y);
}

b2Vec2 vec2b2vec(sf::Vector2f V){
	return b2Vec2(V.x,V.y);
	}
sf::Vector2f b2vec2sfvec(b2Vec2 V){
	return sf::Vector2f(V.x,V.y);
	}
struct Parametres
	{
        float *angle,*torque;
        int *tauxHaptique;
	};
void communicationPC(Parametres p);


int main()
{
	
	//-----------Compteurs pour mesurer la fréquence de calcul----------
	int boucleGraphique(0),tauxGraphique(0),tauxHaptique(0);
	sf::Clock horlogeGraphique, horlogeHaptique;
	//--------------------fenêtre graphique--------------------------------
    sf::RenderWindow window(sf::VideoMode(400, 350),"Haptics");
    window.setPosition(sf::Vector2i(100,50));
    std::ostringstream texteSurImage;

    const float barLength(100);
	sf::Vector2f fulcrum(200,300),effector(0,0);
	sf::Vector2f origine(200,0);
	sf::VertexArray bar(sf::LinesStrip, 2);
	bar[0].position = fulcrum;
	//-------------------------------------------------------------------
	float angle(0),torque(0);
	//---------- Texte à afficher sur l'espace de travail-----------------
    sf::Text textTauxGr, textTauxHa, textAngle, textTorque;
    sf::Font font;
    font.loadFromFile("DejaVuSans.ttf");
    textTauxGr.setPosition(10,10); textTauxGr.setFont(font); textTauxGr.setCharacterSize(10);
    textTauxHa.setPosition(10,20); textTauxHa.setFont(font); textTauxHa.setCharacterSize(10);
    textAngle.setPosition(140,40); textAngle.setFont(font);textAngle.setCharacterSize(15);
    textTorque.setPosition(220,40); textTorque.setFont(font);textTorque.setCharacterSize(15);
	//------------------------------------------------------------------
	
	//----------------Thread du retour d'effort------------------------
	Parametres p = {&angle,&torque,&tauxHaptique};
    sf::Thread threadHaptique(&communicationPC,p);
    threadHaptique.launch();
    
    //------------------------espace de travail-------------------------
    
    //----------------Boucle Graphique---------------------------------
    while (window.isOpen())
    {
		//Mesurer la taux graphique (doit être 60Hz)
		if(horlogeGraphique.getElapsedTime().asMilliseconds() > 1000){
			horlogeGraphique.restart();
			tauxGraphique = boucleGraphique;
			boucleGraphique = 0;
		}else{
			boucleGraphique++;
		}
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||event.type == sf::Event::KeyPressed){
				threadHaptique.terminate();
				window.close();
				}else if(event.type == sf::Event::MouseMoved){				
				//-----------affichage de la position physique du pantographe------
				
				sf::Vector2f mousePos(event.mouseMove.x,event.mouseMove.y);
				sf::Vector2f distance(mousePos - fulcrum); 
				//std::cout <<  "Distance : " << distance.x << " " << distance.y << std::endl;
				angle = atan2(-distance.x, -distance.y);
				effector = fulcrum + distance*(barLength)/norm(distance);
				bar[1].position = effector;
				
			//--------------------------------------------------------------------		
			}else if(event.type == sf::Event::MouseButtonPressed){
			std::cout << "Taux graphique : "  << tauxGraphique << " Hz" << std::endl;
			std::cout <<  "Taux haptique : " << tauxHaptique << " Hz" << std::endl;
			std::cout << "Angle : " << angle*180/pi << "°" << std::endl;
			std::cout << "Couple : " << torque << " mNm" << std::endl;
			}
		}
		texteSurImage.str(std::string());
		texteSurImage << "Frame rate : " << tauxGraphique << " Hz";
		textTauxGr.setString(texteSurImage.str());
		texteSurImage.str(std::string());
		texteSurImage << "Haptic feedback rate : " << tauxHaptique << " Hz";
		textTauxHa.setString(texteSurImage.str());
		texteSurImage.str(std::string());
		texteSurImage << std::fixed << std::setprecision(1) << angle*180/pi << "deg";
		textAngle.setString(texteSurImage.str());
		texteSurImage.str(std::string());
		texteSurImage << std::fixed << std::setprecision(1) << torque << " mNm";
		textTorque.setString(texteSurImage.str());
	    window.clear();
        window.draw(bar);
        window.draw(textTauxGr);
        window.draw(textTauxHa);
        window.draw(textAngle);
        window.draw(textTorque);
        window.display();
    }
    return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void communicationPC(Parametres p){
	float angle(0), torque(0);
	int boucleHaptique(0), tauxHaptique(0);
    sf::Clock horlogeHaptique;
    sf::Time delai;
    delai = sf::microseconds(200);
    
    //comm setup
    sf::TcpListener listener;

	// bind the listener to a port
	if (listener.listen(53200) != sf::Socket::Done)
	{
     error("ERROR on binding");
	}
	
	sf::TcpSocket client;
	if (listener.accept(client) != sf::Socket::Done)
	{
    error("ERROR on accept");
	}
	unsigned char s[2] = "0",r[2] = "0";
    //int encodedAngle(0); //test incrementing
    while(1){
		if(horlogeHaptique.getElapsedTime().asMilliseconds() > 1000){
			horlogeHaptique.restart();
			tauxHaptique = boucleHaptique;			
			boucleHaptique = 0;
			*p.tauxHaptique = tauxHaptique;
	    }else{
			boucleHaptique++;
		}
		//avec Qt
		//encodedAngle+=50;
		//encodedAngle &= 4095;
		int encodedAngle(0);
		angle = *p.angle;
		if(angle < 0) angle += 2*pi;
		encodedAngle = (int)(angle*4096/(2*pi));
		s[0] = (char)(encodedAngle >> 8)&255;
		s[1] = (char)(encodedAngle)&255;
		if(client.send(&s,2) != sf::Socket::Done)
		{
			error("ERROR sending data");
		}//else std::cout << "sent" << std::endl;
		size_t bytesReceived;
		if (client.receive(r,2,bytesReceived) != sf::Socket::Done)
        {
		 error("ERROR reading from socket");
		}        
		else
		{
			int decodedTorque;
			decodedTorque = (int)r[0];
			decodedTorque = (decodedTorque << 8) | (int)r[1];
			torque = (float) (decodedTorque &1023) - 500; 
		}
		/*
		retour.clear();
		if (client.receive(retour) != sf::Socket::Done)
        {
		 error("ERROR reading from socket");
		}        
		else{
		//retour >> couple.x >> couple.y;	
		
	    }*/
		*p.torque = torque;

	    sf::sleep(delai);
	}
}

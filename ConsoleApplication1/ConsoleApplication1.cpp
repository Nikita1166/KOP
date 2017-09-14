#include "stdafx.h"
#include "mas.h"
#include "pos.h"
#include <windows.h>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <math.h>
#include <list>
#include <ctime> 


class Ship	{
public:
	float x, y, w, h, p_x, p_y;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;

	Ship(float X, float Y, float W, float H)	{
		w = W;
		h = H;
		image.loadFromFile("images/ship.png");
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setPosition(p_x, p_y);
		sprite.setOrigin(w / 2, h / 2);
		x = X;
		y = Y;
		sprite.setTextureRect(sf::IntRect((int)x, (int)y, (int)w, (int)h));
	}
};

class marker	{
public:
	float x, y, w, h;
	int pos_x, pos_y;
	bool name;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;

	marker(bool Name, float X, float Y, float H, float W, float Pos_x, float Pos_y) {
		w = W;
		h = H;
		x = X;
		y = Y;
		name = Name;
		pos_x = (int)ceil(Pos_x / 15);
		pos_y = (int)ceil(Pos_y / 15);
		if (pos_x % 2 == 0)	{
			pos_x -= 1;
		}
		pos_x *= 15;
		if (pos_y % 2 == 0)	{
			pos_y -= 1;
		}
		pos_y *= 15;
		if (name == true)	{
			image.loadFromFile("images/marker_b.png");
			texture.loadFromImage(image);
			sprite.setTexture(texture);
		}
		else if (name == false)	{
			image.loadFromFile("images/marker_l.png");
			texture.loadFromImage(image);
			sprite.setTexture(texture);
		}
		sprite.setOrigin(w / 2, h / 2);
		sprite.setPosition((float)pos_x, (float)pos_y);
	}
};

int main()
{
	setlocale(LC_ALL, "rus");
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;
	sf::Packet packet;
	sf::Packet packet_stroke;

	char type, mode = ' ';
	char buffer[2000];
	int time_h = 0, time_g = 0;
	size_t received;
	
	std::string text = "connect to: ";

	std::cout << ip << std::endl;
	std::cout << "Введите время хода (сек):" << std::endl;
	std::cin >> time_h;
	std::cout << "Введите время игры (сек):" << std::endl;
	std::cin >> time_g;
	std::cout << "Введите тип подключения:  c -клиент, s -сервер" << std::endl;
	std::cin >> type;
	if (type == 's'){
		sf::TcpListener listener;
		listener.listen(2000);
		listener.accept(socket);    
		text += "Serwer";
		mode = 's';
	}
	else if (type == 'c'){
		std::cout << "new ip:";
		std::cin >> ip;
		socket.connect(ip, 2000); 
		text += "client";
		mode = 'r';
	}
	socket.send(text.c_str(), text.length() + 1);
	socket.receive(buffer, sizeof(buffer), received);
	std::cout << buffer << std::endl;

	sf::RenderWindow window(sf::VideoMode(816, 585), "Game");
	
	bool flag_Four_deck = false;
	bool flag_Three_deck_1 = false;
	bool flag_Three_deck_2 = false;
	bool flag_Double_deck_1 = false;
	bool flag_Double_deck_2 = false;
	bool flag_Double_deck_3 = false;
	bool flag_Single_deck_1 = false;
	bool flag_Single_deck_2 = false;
	bool flag_Single_deck_3 = false;
	bool flag_Single_deck_4 = false;

	bool flag = false;
	bool flag_turn = false;
	bool flag_move = false;
	bool flag_break = false;
	bool answer = false;
	bool check = false;

	float x = 0.0, y = 0.0;
	int N = 0, p_x = 0, p_y = 0, num = 0, counter_s = 0, counter_r = 0;
	int mas[10][4];

	std::list<marker*>  markers;
	std::list<marker*>::iterator it;

	Ship Four_deck(0, 0, 34, 132);
	Ship Three_deck_1(59, 31, 34, 102);
	Ship Three_deck_2(59, 31, 34, 102);
	Ship Double_deck_1(119, 64, 34, 72);
	Ship Double_deck_2(119, 64, 34, 72);
	Ship Double_deck_3(119, 64, 34, 72);
	Ship Single_deck_1(179, 93, 34, 42);
	Ship Single_deck_2(179, 93, 34, 42);
	Ship Single_deck_3(179, 93, 34, 42);
	Ship Single_deck_4(179, 93, 34, 42);

	float dX_Four_deck = Four_deck.h / 2;
	float dY_Four_deck = Four_deck.w / 2;
	float dX_Three_deck_1 = Three_deck_1.h / 2;
	float dY_Three_deck_1 = Three_deck_1.w / 2;
	float dX_Three_deck_2 = Three_deck_2.h / 2;
	float dY_Three_deck_2 = Three_deck_2.w / 2;
	float dX_Double_deck_1 = Double_deck_1.h / 2;
	float dY_Double_deck_1 = Double_deck_1.w / 2;
	float dX_Double_deck_2 = Double_deck_2.h / 2;
	float dY_Double_deck_2 = Double_deck_2.w / 2;
	float dX_Double_deck_3 = Double_deck_3.h / 2;
	float dY_Double_deck_3 = Double_deck_3.w / 2;
	float dX_Single_deck_1 = Single_deck_1.h / 2;
	float dY_Single_deck_1 = Single_deck_1.w / 2;
	float dX_Single_deck_2 = Single_deck_2.h / 2;
	float dY_Single_deck_2 = Single_deck_2.w / 2;
	float dX_Single_deck_3 = Single_deck_3.h / 2;
	float dY_Single_deck_3 = Single_deck_3.w / 2;
	float dX_Single_deck_4 = Single_deck_4.h / 2;
	float dY_Single_deck_4 = Single_deck_4.w / 2;
	
	sf::Image i_map, i_b, i_l;
	sf::Texture t_map, t_b, t_l;
	sf::Sprite s_map, s_b, s_l;
	i_map.loadFromFile("images/map.png");
	t_map.loadFromImage(i_map);
	s_map.setTexture(t_map);

	while (window.isOpen())
	{
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		sf::Vector2f pos = window.mapPixelToCoords(pixelPos);
		
		sf::Event event;

		unsigned int start_time_g = clock();
		while (window.pollEvent(event))
		{
			unsigned int end_time_g = clock();
			unsigned int search_time_g = end_time_g - start_time_g;
			if (event.type == sf::Event::Closed)
				window.close();
			if (search_time_g > (time_g * 1000))	{
				std::cout << "Время игры закончено" << std::endl;
				std::cout << counter_s << " : " << counter_r << std::endl;
				if (counter_s > counter_r)	{
					std::cout << "Игра закончена вашей победой." << std::endl;
				}
				else if (counter_s > counter_r)	{
					std::cout << "Игра закончена победой противника." << std::endl;
				}
				else if (counter_s = counter_r)	{
					std::cout << "Игра закончена ничьёй." << std::endl;
				}
				window.close();
			}

			if (N == 10)	{
				flag_break = false;
				
				unsigned int start_time = clock();
				while ((event.type == sf::Event::MouseButtonReleased) &&
					(event.key.code == sf::Mouse::Left)) {
					unsigned int end_time = clock();
					unsigned int search_time = end_time - start_time;
					if (search_time > (time_h * 1000))	{
						if (mode == 's')	
							mode = 'r';							
						std::cout << "Время хода вышло" << std::endl;
						x = 1;
						y = 0;
						packet << x << y;
						socket.send(packet);
						flag = false;
						break;
					}
				}
				if (((event.type == sf::Event::MouseButtonPressed) &&
					(event.key.code == sf::Mouse::Left)) && ((pos.x >= 480) && (pos.y >= 155) && (pos.x <= 780) && (pos.y <= 448)))	{
					flag = true;
				}
				
				if (counter_s == 20)	{
					x = 0;
					y = 1;
					packet << x << y;
					socket.send(packet);
					window.close();
					std::cout << "Игра закончена вашей победой." << std::endl;
				}

				if (mode == 's')	{
					if (flag == true)	{ 
						x = pos.x;
						y = pos.y;
						packet << x << y;
						socket.send(packet);
						socket.receive(packet_stroke);
						if (packet_stroke >> answer)	{
							if (answer == true)	{
								markers.push_back(new marker(answer, 0, 0, 30, 30, x, y));
								counter_s++;
								mode = 's';
							}
							else{
								markers.push_back(new marker(answer, 0, 0, 30, 30, x, y));
								mode = 'r';
							}
							packet.clear();
							packet_stroke.clear();
						}
						
					}
				}
				else {
					(socket.receive(packet));
					if (packet >> x >> y)	{
						if (y == 1)	{
							window.close();
							std::cout << "Игра закончена победой противника." << std::endl;
						}
						else if (x == 1)	{ mode = 's'; }
						else
						{
							x -= 420;
							for (int i = 0; i < 10; i++)	{
								if ((x >= mas[i][0]) && (x <= mas[i][2]) &&
									(y >= mas[i][1]) && (y <= mas[i][3]))	{
									flag_break = true;
									markers.push_back(new marker(flag_break, 0, 0, 30, 30, x, y));
									counter_r++;
									packet_stroke << true;
									socket.send(packet_stroke);
									mode = 'r';
									break;
								}
							}

							if (flag_break == false)	{
								markers.push_back(new marker(flag_break, 0, 0, 30, 30, x, y));
								packet_stroke << false;
								socket.send(packet_stroke);
								mode = 's';
							}
						}
						packet.clear();
						packet_stroke.clear();
					}
				}
				flag = false;
			}
						
			if ((N != 10) && (event.type == sf::Event::MouseButtonPressed) &&
				(event.key.code == sf::Mouse::Left) && (((pos.x > 0) && (pos.y > 0)) && ((pos.x < 210) && (pos.y < 30))))	{
				N = 0;
				flag_Four_deck = false;
				flag_Three_deck_1 = false;
				flag_Three_deck_2 = false;
				flag_Double_deck_1 = false;
				flag_Double_deck_2 = false;
				flag_Double_deck_3 = false;
				flag_Single_deck_1 = false;
				flag_Single_deck_2 = false;
				flag_Single_deck_3 = false;
				flag_Single_deck_4 = false;
				for (int i = 0; i < 10; i++)	{
					for (int j = 0; j < 4; j++)	{
						mas[i][j] = 0;
					}
				}
			}
			
			switch (N)	{
			case 0:
				if ((flag_turn == false) && (((pos.x > 74) && (pos.y > 210)) && ((pos.x < 346) && (pos.y < 393))))	{
					Four_deck.sprite.setPosition(pos.x, pos.y);
					dX_Four_deck = pos.x - Four_deck.sprite.getPosition().x;
					dY_Four_deck = pos.y - Four_deck.sprite.getPosition().y;
					flag_Four_deck = true;
				}
				else if((flag_turn == true) && (((pos.x > 117) && (pos.y > 164)) && ((pos.x < 299) && (pos.y < 435))))	{
					Four_deck.sprite.setPosition(pos.x, pos.y);
					dX_Four_deck = pos.x - Four_deck.sprite.getPosition().x;
					dY_Four_deck = pos.y - Four_deck.sprite.getPosition().y;
					flag_Four_deck = true;
				}
				else {
					flag_Four_deck = false;
				}
				if ((event.type == sf::Event::MouseButtonPressed) &&
					(event.key.code == sf::Mouse::Left) && (flag_Four_deck == true) && (flag_turn == false))	{
					p_x = position_x((int)pos.x, flag_turn);
					p_y = position_y((int)pos.y, flag_turn);
					Four_deck.sprite.setPosition((float)p_x, (float)p_y);
					mas[N][0] = (int)Four_deck.sprite.getPosition().x - 15;
					mas[N][1] = (int)Four_deck.sprite.getPosition().y - 60;
					mas[N][2] = (int)Four_deck.sprite.getPosition().x + 15;
					mas[N][3] = (int)Four_deck.sprite.getPosition().y + 60;
					check = massiv(mas[N][0], mas[N][1], mas[N][2], mas[N][3], N);
					if (check == true)	{
						N++;
					}
				}
				else if ((event.type == sf::Event::MouseButtonPressed) &&
					(event.key.code == sf::Mouse::Left) && (flag_Four_deck == true) && (flag_turn == true))	{
					p_x = position_x((int)pos.x, flag_turn);
					p_y = position_y((int)pos.y, flag_turn);
					Four_deck.sprite.setPosition((float)p_x, (float)p_y);
					mas[N][0] = (int)Four_deck.sprite.getPosition().x - 60;
					mas[N][1] = (int)Four_deck.sprite.getPosition().y - 15;
					mas[N][2] = (int)Four_deck.sprite.getPosition().x + 60;
					mas[N][3] = (int)Four_deck.sprite.getPosition().y + 15;
					check = massiv(mas[N][0], mas[N][1], mas[N][2], mas[N][3], N);
					if (check == true)	{
						N++;
					}
				}
				break;
			case 1:
				if ((flag_turn == false) && (((pos.x > 74) && (pos.y > 197)) && ((pos.x < 346) && (pos.y < 407))))	{
					Three_deck_1.sprite.setPosition(pos.x, pos.y);
					dX_Three_deck_1 = pos.x - Three_deck_1.sprite.getPosition().x;
					dY_Three_deck_1 = pos.y - Three_deck_1.sprite.getPosition().y;
					flag_Three_deck_1 = true;
				}
				else if((flag_turn == true) && (((pos.x > 102) && (pos.y > 164)) && ((pos.x < 313) && (pos.y < 435))))	{
					Three_deck_1.sprite.setPosition(pos.x, pos.y);
					dX_Three_deck_1 = pos.x - Three_deck_1.sprite.getPosition().x;
					dY_Three_deck_1 = pos.y - Three_deck_1.sprite.getPosition().y;
					flag_Three_deck_1 = true;
				}
				else {
					flag_Three_deck_1 = false;
				}
				
				if ((event.type == sf::Event::MouseButtonPressed) &&
					(event.key.code == sf::Mouse::Left) && (flag_Three_deck_1 == true) && (flag_turn == false))	{
					p_x = position_x((int)pos.x, flag_turn);
					p_y = position_y((int)pos.y, !flag_turn);
					
					Three_deck_1.sprite.setPosition((float)p_x, (float)p_y);
					mas[N][0] = (int)Three_deck_1.sprite.getPosition().x - 15;
					mas[N][1] = (int)Three_deck_1.sprite.getPosition().y - 45;
					mas[N][2] = (int)Three_deck_1.sprite.getPosition().x + 15;
					mas[N][3] = (int)Three_deck_1.sprite.getPosition().y + 45;
					check = massiv(mas[N][0], mas[N][1], mas[N][2], mas[N][3], N);
					if (check == true)	{
						N++;
					}
				}
				else if ((event.type == sf::Event::MouseButtonPressed) &&
					(event.key.code == sf::Mouse::Left) && (flag_Three_deck_1 == true) && (flag_turn == true))	{
					p_x = position_x((int)pos.x, !flag_turn);
					p_y = position_y((int)pos.y, flag_turn);
					
					Three_deck_1.sprite.setPosition((float)p_x, (float)p_y);
					mas[N][0] = (int)Three_deck_1.sprite.getPosition().x - 45;
					mas[N][1] = (int)Three_deck_1.sprite.getPosition().y - 15;
					mas[N][2] = (int)Three_deck_1.sprite.getPosition().x + 45;
					mas[N][3] = (int)Three_deck_1.sprite.getPosition().y + 15;
					check = massiv(mas[N][0], mas[N][1], mas[N][2], mas[N][3], N);
					if (check == true)	{
						N++;
					}
				}
				break;
			case 2:
				if ((flag_turn == false) && (((pos.x > 74) && (pos.y > 197)) && ((pos.x < 346) && (pos.y < 407))))	{
					Three_deck_2.sprite.setPosition(pos.x, pos.y);
					dX_Three_deck_2 = pos.x - Three_deck_2.sprite.getPosition().x;
					dY_Three_deck_2 = pos.y - Three_deck_2.sprite.getPosition().y;
					flag_Three_deck_2 = true;
				}
				else if ((flag_turn == true) && (((pos.x > 102) && (pos.y > 164)) && ((pos.x < 313) && (pos.y < 435))))	{
					Three_deck_2.sprite.setPosition(pos.x, pos.y);
					dX_Three_deck_2 = pos.x - Three_deck_2.sprite.getPosition().x;
					dY_Three_deck_2 = pos.y - Three_deck_2.sprite.getPosition().y;
					flag_Three_deck_2 = true;
				}
				else {
					flag_Three_deck_2 = false;
				}
				if ((event.type == sf::Event::MouseButtonPressed) &&
					(event.key.code == sf::Mouse::Left) && (flag_Three_deck_2 == true) && (flag_turn == false))	{
					p_x = position_x((int)pos.x, flag_turn);
					p_y = position_y((int)pos.y, !flag_turn);
					
					Three_deck_2.sprite.setPosition((float)p_x, (float)p_y);
					mas[N][0] = (int)Three_deck_2.sprite.getPosition().x - 15;
					mas[N][1] = (int)Three_deck_2.sprite.getPosition().y - 45;
					mas[N][2] = (int)Three_deck_2.sprite.getPosition().x + 15;
					mas[N][3] = (int)Three_deck_2.sprite.getPosition().y + 45;
					check = massiv(mas[N][0], mas[N][1], mas[N][2], mas[N][3], N);
					if (check == true)	{
						N++;
					}
				}
				else if ((event.type == sf::Event::MouseButtonPressed) &&
					(event.key.code == sf::Mouse::Left) && (flag_Three_deck_2 == true) && (flag_turn == true))	{
					p_x = position_x((int)pos.x, !flag_turn);
					p_y = position_y((int)pos.y, flag_turn);
					
					Three_deck_2.sprite.setPosition((float)p_x, (float)p_y);
					mas[N][0] = (int)Three_deck_2.sprite.getPosition().x - 45;
					mas[N][1] = (int)Three_deck_2.sprite.getPosition().y - 15;
					mas[N][2] = (int)Three_deck_2.sprite.getPosition().x + 45;
					mas[N][3] = (int)Three_deck_2.sprite.getPosition().y + 15;
					check = massiv(mas[N][0], mas[N][1], mas[N][2], mas[N][3], N);
					if (check == true)	{
						N++;
					}
				}
				break;
			case 3:
				if ((flag_turn == false) && (((pos.x > 74) && (pos.y > 183)) && ((pos.x < 346) && (pos.y < 425))))	{
					Double_deck_1.sprite.setPosition(pos.x, pos.y);
					dX_Double_deck_1 = pos.x - Double_deck_1.sprite.getPosition().x;
					dY_Double_deck_1 = pos.y - Double_deck_1.sprite.getPosition().y;
					flag_Double_deck_1 = true;
				}
				else if((flag_turn == true) && (((pos.x > 85) && (pos.y > 164)) && ((pos.x < 327) && (pos.y < 435))))	{
					Double_deck_1.sprite.setPosition(pos.x, pos.y);
					dX_Double_deck_1 = pos.x - Double_deck_1.sprite.getPosition().x;
					dY_Double_deck_1 = pos.y - Double_deck_1.sprite.getPosition().y;
					flag_Double_deck_1 = true;
				}
				else {
					flag_Double_deck_1 = false;
				}
				if ((event.type == sf::Event::MouseButtonPressed) &&
					(event.key.code == sf::Mouse::Left) && (flag_Double_deck_1 == true) && (flag_turn == false))	{
					p_x = position_x((int)pos.x, flag_turn);
					p_y = position_y((int)pos.y, flag_turn);
					
					Double_deck_1.sprite.setPosition((float)p_x, (float)p_y);
					mas[N][0] = (int)Double_deck_1.sprite.getPosition().x - 15;
					mas[N][1] = (int)Double_deck_1.sprite.getPosition().y - 30;
					mas[N][2] = (int)Double_deck_1.sprite.getPosition().x + 15;
					mas[N][3] = (int)Double_deck_1.sprite.getPosition().y + 30;
					check = massiv(mas[N][0], mas[N][1], mas[N][2], mas[N][3], N);
					if (check == true)	{
						N++;
					}
				}
				else if ((event.type == sf::Event::MouseButtonPressed) &&
					(event.key.code == sf::Mouse::Left) && (flag_Double_deck_1 == true) && (flag_turn == true))	{
					p_x = position_x((int)pos.x, flag_turn);
					p_y = position_y((int)pos.y, flag_turn);
					
					Double_deck_1.sprite.setPosition((float)p_x, (float)p_y);
					mas[N][0] = (int)Double_deck_1.sprite.getPosition().x - 30;
					mas[N][1] = (int)Double_deck_1.sprite.getPosition().y - 15;
					mas[N][2] = (int)Double_deck_1.sprite.getPosition().x + 30;
					mas[N][3] = (int)Double_deck_1.sprite.getPosition().y + 15;
					check = massiv(mas[N][0], mas[N][1], mas[N][2], mas[N][3], N);
					if (check == true)	{
						N++;
					}
				}
				break;
			case 4:
				if ((flag_turn == false) && (((pos.x > 74) && (pos.y > 183)) && ((pos.x < 346) && (pos.y < 425))))	{
					Double_deck_2.sprite.setPosition(pos.x, pos.y);
					dX_Double_deck_2 = pos.x - Double_deck_2.sprite.getPosition().x;
					dY_Double_deck_2 = pos.y - Double_deck_2.sprite.getPosition().y;
					flag_Double_deck_2 = true;
				}
				else if ((flag_turn == true) && (((pos.x > 85) && (pos.y > 164)) && ((pos.x < 327) && (pos.y < 435))))	{
					Double_deck_2.sprite.setPosition(pos.x, pos.y);
					dX_Double_deck_2 = pos.x - Double_deck_2.sprite.getPosition().x;
					dY_Double_deck_2 = pos.y - Double_deck_2.sprite.getPosition().y;
					flag_Double_deck_2 = true;
				}
				else {
					flag_Double_deck_2 = false;
				}
				if ((event.type == sf::Event::MouseButtonPressed) &&
					(event.key.code == sf::Mouse::Left) && (flag_Double_deck_2 == true) && (flag_turn == false))	{
					p_x = position_x((int)pos.x, flag_turn);
					p_y = position_y((int)pos.y, flag_turn);
					
					Double_deck_2.sprite.setPosition((float)p_x, (float)p_y);
					mas[N][0] = (int)Double_deck_2.sprite.getPosition().x - 15;
					mas[N][1] = (int)Double_deck_2.sprite.getPosition().y - 30;
					mas[N][2] = (int)Double_deck_2.sprite.getPosition().x + 15;
					mas[N][3] = (int)Double_deck_2.sprite.getPosition().y + 30;
					check = massiv(mas[N][0], mas[N][1], mas[N][2], mas[N][3], N);
					if (check == true)	{
						N++;
					}
				}
				else if ((event.type == sf::Event::MouseButtonPressed) &&
					(event.key.code == sf::Mouse::Left) && (flag_Double_deck_2 == true) && (flag_turn == true))	{
					p_x = position_x((int)pos.x, flag_turn);
					p_y = position_y((int)pos.y, flag_turn);
					
					Double_deck_2.sprite.setPosition((float)p_x, (float)p_y);
					mas[N][0] = (int)Double_deck_2.sprite.getPosition().x - 30;
					mas[N][1] = (int)Double_deck_2.sprite.getPosition().y - 15;
					mas[N][2] = (int)Double_deck_2.sprite.getPosition().x + 30;
					mas[N][3] = (int)Double_deck_2.sprite.getPosition().y + 15;
					check = massiv(mas[N][0], mas[N][1], mas[N][2], mas[N][3], N);
					if (check == true)	{
						N++;
					}
				}
				break;
			case 5:
				if ((flag_turn == false) && (((pos.x > 74) && (pos.y > 183)) && ((pos.x < 346) && (pos.y < 425))))	{
					Double_deck_3.sprite.setPosition(pos.x, pos.y);
					dX_Double_deck_3 = pos.x - Double_deck_3.sprite.getPosition().x;
					dY_Double_deck_3 = pos.y - Double_deck_3.sprite.getPosition().y;
					flag_Double_deck_3 = true;
				}
				else if ((flag_turn == true) && (((pos.x > 85) && (pos.y > 164)) && ((pos.x < 327) && (pos.y < 435))))	{
					Double_deck_3.sprite.setPosition(pos.x, pos.y);
					dX_Double_deck_3 = pos.x - Double_deck_3.sprite.getPosition().x;
					dY_Double_deck_3 = pos.y - Double_deck_3.sprite.getPosition().y;
					flag_Double_deck_3 = true;
				}
				else {
					flag_Double_deck_3 = false;
				}
				if ((event.type == sf::Event::MouseButtonPressed) &&
					(event.key.code == sf::Mouse::Left) && (flag_Double_deck_3 == true) && (flag_turn == false))	{
					p_x = position_x((int)pos.x, flag_turn);
					p_y = position_y((int)pos.y, flag_turn);
					
					Double_deck_3.sprite.setPosition((float)p_x, (float)p_y);
					mas[N][0] = (int)Double_deck_3.sprite.getPosition().x - 15;
					mas[N][1] = (int)Double_deck_3.sprite.getPosition().y - 30;
					mas[N][2] = (int)Double_deck_3.sprite.getPosition().x + 15;
					mas[N][3] = (int)Double_deck_3.sprite.getPosition().y + 30;
					check = massiv(mas[N][0], mas[N][1], mas[N][2], mas[N][3], N);
					if (check == true)	{
						N++;
					}
				}
				else if ((event.type == sf::Event::MouseButtonPressed) &&
					(event.key.code == sf::Mouse::Left) && (flag_Double_deck_3 == true) && (flag_turn == true))	{
					p_x = position_x((int)pos.x, flag_turn);
					p_y = position_y((int)pos.y, flag_turn);
					check = massiv(mas[N][0], mas[N][1], mas[N][2], mas[N][3], N);
					if (check == true)	{
						Double_deck_3.sprite.setPosition((float)p_x, (float)p_y);
						mas[N][0] = (int)Double_deck_3.sprite.getPosition().x - 30;
						mas[N][1] = (int)Double_deck_3.sprite.getPosition().y - 15;
						mas[N][2] = (int)Double_deck_3.sprite.getPosition().x + 30;
						mas[N][3] = (int)Double_deck_3.sprite.getPosition().y + 15;
						N++;
					}
				}
				break;
			case 6:
				if (((pos.x > 74) && (pos.y > 168)) && ((pos.x < 345) && (pos.y < 438)))	{
					Single_deck_1.sprite.setPosition(pos.x, pos.y);
					dX_Single_deck_1 = pos.x - Single_deck_1.sprite.getPosition().x;
					dY_Single_deck_1 = pos.y - Single_deck_1.sprite.getPosition().y;
					flag_Single_deck_1 = true;
				}
				else {
					flag_Single_deck_1 = false;
				}
				if ((event.type == sf::Event::MouseButtonPressed) &&
					(event.key.code == sf::Mouse::Left) && (flag_Single_deck_1 == true))	{
					p_x = position_x((int)pos.x, false);
					p_y = position_y((int)pos.y, true);
					
					Single_deck_1.sprite.setPosition((float)p_x, (float)p_y);
					mas[N][0] = (int)Single_deck_1.sprite.getPosition().x - 15;
					mas[N][1] = (int)Single_deck_1.sprite.getPosition().y - 15;
					mas[N][2] = (int)Single_deck_1.sprite.getPosition().x + 15;
					mas[N][3] = (int)Single_deck_1.sprite.getPosition().y + 15;
					check = massiv(mas[N][0], mas[N][1], mas[N][2], mas[N][3], N);
					if (check == true)	{
						N++;
					}
				}
				break;
			case 7:
				if (((pos.x > 74) && (pos.y > 168)) && ((pos.x < 345) && (pos.y < 438)))	{
					Single_deck_2.sprite.setPosition(pos.x, pos.y);
					dX_Single_deck_2 = pos.x - Single_deck_2.sprite.getPosition().x;
					dY_Single_deck_2 = pos.y - Single_deck_2.sprite.getPosition().y;
					flag_Single_deck_2 = true;
				}
				else {
					flag_Single_deck_2 = false;
				}
				if ((event.type == sf::Event::MouseButtonPressed) &&
					(event.key.code == sf::Mouse::Left) && (flag_Single_deck_2 == true))	{
					p_x = position_x((int)pos.x, false);
					p_y = position_y((int)pos.y, true);
					
					Single_deck_2.sprite.setPosition((float)p_x, (float)p_y);
					mas[N][0] = (int)Single_deck_2.sprite.getPosition().x - 15;
					mas[N][1] = (int)Single_deck_2.sprite.getPosition().y - 15;
					mas[N][2] = (int)Single_deck_2.sprite.getPosition().x + 15;
					mas[N][3] = (int)Single_deck_2.sprite.getPosition().y + 15;
					check = massiv(mas[N][0], mas[N][1], mas[N][2], mas[N][3], N);
					if (check == true)	{
						N++;
					}
				}
				break;
			case 8:
				if (((pos.x > 74) && (pos.y > 168)) && ((pos.x < 345) && (pos.y < 438)))	{
					Single_deck_3.sprite.setPosition(pos.x, pos.y);
					dX_Single_deck_3 = pos.x - Single_deck_3.sprite.getPosition().x;
					dY_Single_deck_3 = pos.y - Single_deck_3.sprite.getPosition().y;
					flag_Single_deck_3 = true;
				}
				else {
					flag_Single_deck_3 = false;
				}
				if ((event.type == sf::Event::MouseButtonPressed) &&
					(event.key.code == sf::Mouse::Left) && (flag_Single_deck_3 == true))	{
					p_x = position_x((int)pos.x, false);
					p_y = position_y((int)pos.y, true);
				
					Single_deck_3.sprite.setPosition((float)p_x, (float)p_y);
					mas[N][0] = (int)Single_deck_3.sprite.getPosition().x - 15;
					mas[N][1] = (int)Single_deck_3.sprite.getPosition().y - 15;
					mas[N][2] = (int)Single_deck_3.sprite.getPosition().x + 15;
					mas[N][3] = (int)Single_deck_3.sprite.getPosition().y + 15;
					check = massiv(mas[N][0], mas[N][1], mas[N][2], mas[N][3], N);
					if (check == true)	{
						N++;
					}
				}
				break;
			case 9:
				if (((pos.x > 74) && (pos.y > 168)) && ((pos.x < 345) && (pos.y < 438)))	{
					Single_deck_4.sprite.setPosition(pos.x, pos.y);
					dX_Single_deck_4 = pos.x - Single_deck_4.sprite.getPosition().x;
					dY_Single_deck_4 = pos.y - Single_deck_4.sprite.getPosition().y;
					flag_Single_deck_4 = true;
				}
				else {
					flag_Single_deck_4 = false;
				}
				if ((event.type == sf::Event::MouseButtonPressed) &&
					(event.key.code == sf::Mouse::Left) && (flag_Single_deck_4 == true))	{
					p_x = position_x((int)pos.x, false);
					p_y = position_y((int)pos.y, true);
					
					Single_deck_4.sprite.setPosition((float)p_x, (float)p_y);
					mas[N][0] = (int)Single_deck_4.sprite.getPosition().x - 15;
					mas[N][1] = (int)Single_deck_4.sprite.getPosition().y - 15;
					mas[N][2] = (int)Single_deck_4.sprite.getPosition().x + 15;
					mas[N][3] = (int)Single_deck_4.sprite.getPosition().y + 15;
					check = massiv(mas[N][0], mas[N][1], mas[N][2], mas[N][3], N);
					if (check == true)	{
						N++;
					}
				}
				break;
			}

		}	

		if ((flag_move == false) && ((event.type == sf::Event::MouseButtonPressed) &&
			(event.key.code == sf::Mouse::Right)))	{
			if (flag_turn == false)	{
				if (N == 0)	{ Four_deck.sprite.setRotation(90); }
				if (N <= 1)	{ Three_deck_1.sprite.setRotation(90); }
				if (N <= 2)	{ Three_deck_2.sprite.setRotation(90); }
				if (N <= 3)	{ Double_deck_1.sprite.setRotation(90); }
				if (N <= 4)	{ Double_deck_2.sprite.setRotation(90); }
				if (N <= 5)	{ Double_deck_3.sprite.setRotation(90); }
				flag_turn = true;
				flag_move = true;
			}
			else if (flag_turn == true)	{
				if (N == 0)	{ Four_deck.sprite.setRotation(0); }
				if (N <= 1)	{ Three_deck_1.sprite.setRotation(0); }
				if (N <= 2)	{ Three_deck_2.sprite.setRotation(0); }
				if (N <= 3)	{ Double_deck_1.sprite.setRotation(0); }
				if (N <= 4)	{ Double_deck_2.sprite.setRotation(0); }
				if (N <= 5)	{ Double_deck_3.sprite.setRotation(0); }
				flag_turn = false;
				flag_move = true;
			}
		}
		if ((flag_move == true) && ((event.type == sf::Event::MouseButtonReleased) &&
			(event.key.code == sf::Mouse::Right)))	{
			flag_move = false;
		}

		window.draw(s_map);
		if (flag_Four_deck == true)	{
			window.draw(Four_deck.sprite);
		}
		if (flag_Three_deck_1 == true)	{
			window.draw(Three_deck_1.sprite);
		}
		if (flag_Three_deck_2 == true)	{
			window.draw(Three_deck_2.sprite);
		}
		if (flag_Double_deck_1 == true)	{
			window.draw(Double_deck_1.sprite);
		}
		if (flag_Double_deck_2 == true)	{
			window.draw(Double_deck_2.sprite);
		}
		if (flag_Double_deck_3 == true)	{
			window.draw(Double_deck_3.sprite);
		}
		if (flag_Single_deck_1 == true)	{
			window.draw(Single_deck_1.sprite);
		}
		if (flag_Single_deck_2 == true)	{
			window.draw(Single_deck_2.sprite);
		}
		if (flag_Single_deck_3 == true)	{
			window.draw(Single_deck_3.sprite);
		}
		if (flag_Single_deck_4 == true)	{
			window.draw(Single_deck_4.sprite);
		}
		
		for (it = markers.begin(); it != markers.end(); it++)	{
			window.draw((*it)->sprite);
		}
		
		window.display();
	}

	return 0;
}
#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;//�������� sfml ������ "���", ������� � �������� �������

void getplayercoordinateforview(float x, float y) { //������� ��� ���������� ��������� ������
	float tempX = x; float tempY = y;//��������� ����� ������ � ��������� ��, ����� ������ ����

	if (x < -1500) tempX = -1500;//������� �� ���� ����� �������
	if (y < -540) tempY = -540;//������� �������
	if (y > 1550) tempY = 1550;//������ �������	
	if (x > 1500) tempX = 1500;//������� �� ���� ����� �������

	view.setCenter(tempX, tempY); //������ �� �������, ��������� ��� ����������. 

}


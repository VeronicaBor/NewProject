#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;//�������� sfml ������ "���", ������� � �������� �������

void getplayercoordinateforview(float x, float y) { //������� ��� ���������� ��������� ������
	float tempX = x; float tempY = y;//��������� ����� ������ � ��������� ��, ����� ������ ����

	if (x < -520) tempX = -520;//������� �� ���� ����� �������
	if (y < -540) tempY = -540;//������� �������
	if (y > 1054) tempY = 1054;//������ �������	

	view.setCenter(tempX, tempY); //������ �� �������, ��������� ��� ����������. 

}
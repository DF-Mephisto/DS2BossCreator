#pragma once

#include <QtWidgets>

class DrawingsNet : public QWidget
{
	Q_OBJECT

public:
	DrawingsNet(QWidget *parent = Q_NULLPTR);
	~DrawingsNet();

	void setBlockCount(int xCount, int yCount, int layers);
	void setCurrentLayer(int layer);
	bool*** getBullets();
	int getTotalBullets();

	int getXCount();
	int getYCount();
	int getLayers();

private:
	int xCount;
	int yCount;
	int layer;
	int layers;
	int curBlockSize;

	int bulletsLeft;

	bool*** bullets;

	const int blockSize;
	const int maxBullets;

protected:
	void paintEvent(QPaintEvent* e) override;
	void mousePressEvent(QMouseEvent* e) override;

signals:
	void changedBulletsLeft(int count);

};
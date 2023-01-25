#include <SFML/Graphics.hpp>

using namespace sf; //Чтобы использовать сокращения

int main()
{
	RenderWindow window(VideoMode(1600, 1065), "Arkanoid");//открываем окно

	/*Создаем правильный фон приложения*/
	Texture textureBackground;
	textureBackground.loadFromFile("Background.jpg");
	Sprite background;
	background.setTexture(textureBackground);

	/*Создаем кнопки Play и Quit*/
	Texture texturePlay; //Картинка (любой формы)
	texturePlay.loadFromFile("Play.png");
	Sprite buttonPlay; //Прямоугольник с картинкой (Как Геральд)
	buttonPlay.setTexture(texturePlay);
	buttonPlay.setPosition(1400, 600);

	Texture textureQuit;
	textureQuit.loadFromFile("Quit.png");
	Sprite buttonQuit;
	buttonQuit.setTexture(textureQuit);
	buttonQuit.setPosition(1400, 700);

	/*Создаем платформу*/
	Texture texturePlatform;
	texturePlatform.loadFromFile("Platform.png");
	Sprite platform;
	platform.setTexture(texturePlatform);
	int platformX = 681;

	bool isPuskGame = false; //Переменная, отвечающая за нажатие кнопки
	bool isGame = false; //Переменная, отвечающая за ход игры
	bool isWin = false; //Переменная, отвечающая за победу
	bool isLoss = false; //Переменная, отвечающая за поражение

	/*Создаем линию*/
	RectangleShape line;
	line.setSize(Vector2f(1600, 1));
	line.setFillColor(Color::White);
	line.setPosition(0, 100);

	/*Вставляем текстуру блоков*/
	Texture textureBlock;
	textureBlock.loadFromFile("Block.png");
	Sprite block[24];

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 3; j++) {
			block[j * 8 + i].setTexture(textureBlock);
			block[j * 8 + i].setTextureRect(IntRect(0, 0, 140, 50));
		}
	int brokenBlocks = 0;

	/*Создаем мячик*/
	Texture textureBall;
	textureBall.loadFromFile("ball.png");
	Sprite ball;
	ball.setTexture(textureBall);
	int ballX = 1; int ballY = 1;
	bool isBallMove = false;

	/*Вставляем надпись победы*/
	Texture textureWin;
	textureWin.loadFromFile("win.png");
	Sprite win;
	win.setTexture(textureWin);
	win.setPosition(630, 450);

	/*Вставляем надпись поражения*/
	Texture textureLoss;
	textureLoss.loadFromFile("win.png");
	Sprite loss;
	loss.setTexture(textureLoss);
	loss.setPosition(630, 450);

	/*Жизни*/
	Sprite life[3];
	for (int i = 0; i < 3; i++) {
		life[i].setTexture(textureBall);
		life[i].setPosition(10 + 100 * i, 10);
		life[i].setScale(0.9, 0.9);
	}
	int countLife = 3;

	Font font;
	font.loadFromFile("CyrilicOld.ttf");
	Text text("Score :", font, 50);
	text.setPosition(1400, 25);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::MouseButtonPressed)
				if (event.key.code == Mouse::Left) {

					Vector2i pos = Mouse::getPosition(window);

					if (pos.x >= 1400 && pos.x <= 1529 &&
						pos.y >= 700 && pos.y <= 769)
						window.close();

					if (pos.x >= 1400 && pos.x <= 1529 &&
						pos.y >= 600 && pos.y <= 669 && isPuskGame == false)
						isPuskGame = true;
				}
			if (event.type == Event::KeyPressed)
				if (Keyboard::isKeyPressed(Keyboard::Space)) {
					if (isGame && !isBallMove)
						isBallMove = true;
				}

		}

		window.draw(background);
		window.draw(buttonQuit);
		window.draw(buttonPlay);

		/*Если нажали на кнопку Play, назначаем везде стартовые позиции*/
		if (isPuskGame)
		{
			isWin = false;
			isGame = true;
			isLoss = false;

			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 3; j++)
					block[j * 8 + i].setPosition(25 + 200 * i, 100 + 100 * (j + 1));

			brokenBlocks = 0;

			countLife = 3;

			platform.setPosition(681, 900);
			ball.setPosition(750, 816);

			isPuskGame = false;
		}

		//Движение платформы
		if (isGame)
		{
			if (Keyboard::isKeyPressed(Keyboard::Right))
				if (platformX < 1363) {
					platform.move(3, 0);
					platformX += 3;
					if (!isBallMove)
					{
						ball.move(3, 0);
					}
				}
			if (Keyboard::isKeyPressed(Keyboard::Left))
				if (platformX > 0) {
					platform.move(-3, 0);
					platformX -= 3;
					if (!isBallMove)
					{
						ball.move(-3, 0);
					}
				}

		}

		/*Если игра началась, вырисовываем нужные спрайты*/
		if (isGame)
		{
			window.draw(line);
			window.draw(platform);
			window.draw(ball);
			window.draw(text);

			for (int i = 0; i < 24; i++)
			{
				window.draw(block[i]);
			}

			for (int i = 0; i < countLife; i++)
				window.draw(life[i]);


		}

		if (isBallMove && isGame)
			ball.move((0.75 + brokenBlocks / 50) * ballX, (0.75 + brokenBlocks / 50) * ballY);
		Vector2f vecPlatform = platform.getPosition();
		Vector2f vecBall = ball.getPosition();

		if (vecBall.y < 100 && isGame) //Мяч отбивается от стен
			ballY *= -1;
		if (vecBall.x > 1500 || vecBall.x < 0)
			ballX *= -1;

		//Платформа отбивает мяч
		if (ball.getGlobalBounds().intersects(platform.getGlobalBounds())) {
			ballY *= -1;

			ball.move(0, -0.5);
		}

		//Удаление блоков при соприкосновении с мячиком
		for (int i = 0; i < 24; i++) {
			if (ball.getGlobalBounds().intersects(block[i].getGlobalBounds())) {
				ballY *= -1;
				ball.move(0, 0.1 * ballY);
				block[i].setPosition(1700, 0);
				brokenBlocks++;
				text.setString("Score: " + std::to_string(brokenBlocks));
			}
		}

		if (vecBall.y > 900) {
			isBallMove = false;
			countLife--;

			platform.setPosition(681, 900);
			ball.setPosition(750, 816);
		}

		if (brokenBlocks == 48) {
			isWin = true;
			isGame = false;
		}

		if (isWin) {
			window.draw(win);
		}

		if (countLife < 1) {
			isLoss = true;
			isGame = false;
		}

		if (isLoss) {
			window.draw(loss);
		}


		window.display();

	}

	return 0;
}
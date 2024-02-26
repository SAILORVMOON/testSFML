#include <SFML/Graphics.hpp>

class ArrayList {
public:
	ArrayList() {
		capacity = 0;
		length = 0;
		mas = new sf::RectangleShape* [0];
		speed = new int [0];
	}
	ArrayList(int i) {
		capacity = i;
		length = 0;
		mas = new sf::RectangleShape* [i];
		speed = new int [i];
	}

	void clear() {
		for (int i = 0; i < length; i++) {
			delete mas[i];
		}
		delete[] mas;
		capacity = 0;
		length = 0;
		mas = new sf::RectangleShape* [0];
	}
	void del(int index) {
		delete pop(index);
	}
	sf::RectangleShape* get(int index) {
		return mas[index];
	}
	int getCapacity() const {
		return capacity;
	}
	int getLength() const {
		return length;
	}
	sf::RectangleShape* pop(int index) {
		sf::RectangleShape* temp = mas[index];
		for (int i = index; i < length - 1; i++) {
			mas[i] = mas[i + 1];
		}
		for (int i = index; i < length - 1; i++) {
			speed[i] = speed[i + 1];
		}
		length--;
		return temp;
	}
	void push(sf::RectangleShape* n, int i) {
		if (capacity == length) {
			makeLonger();
		}
		mas[length] = n;
		speed[length] = i + 1;
		length++;
	}
	void move() {
		if (length == 0) {
			return;
		}
		for (int i = 0; i < length; i++) {
			(*mas[i]).setPosition(sf::Vector2f((*mas[i]).getPosition().x , (*mas[i]).getPosition().y + speed[i] * 7));
		}
	}

private:
	int capacity, length;
	sf::RectangleShape** mas;
	int* speed;

	void makeLonger() {
		sf::RectangleShape** masTemp = new sf::RectangleShape* [int(capacity * 1.5) + 1];
		for (int i = 0; i < capacity; i++) {
			masTemp[i] = mas[i];
		}
		delete[] mas;
		mas = masTemp;

		int *tempSpeed = new int [int(capacity * 1.5) + 1];
		for (int i = 0; i < capacity; i++) {
			tempSpeed[i] = speed[i];
		}
		delete[] speed;
		speed = tempSpeed;

		capacity = int(capacity * 1.5) + 1;
	}
};

void drawAll(sf::RectangleShape* player, ArrayList* arr, sf::RenderWindow* window) {
	(*window).clear(sf::Color::Black);
	(*window).draw((*player));
	for (int i = 0; i < (*arr).getLength(); i++) {
		(*window).draw((*(*arr).get(i)));
	}
	
}
void clear(ArrayList* arr) {
	for (int i = 0; i < (*arr).getLength(); i++) {
		if ((*(*arr).get(i)).getPosition().y > 900) {
			(*arr).del(i);
		}
	}
}

bool collision(sf::RectangleShape* sh, sf::RectangleShape* player) {
	if ((*sh).getPosition().y + (*sh).getSize().y < (*player).getPosition().y ||
		(*sh).getPosition().y > (*player).getPosition().y + (*player).getSize().y) {
		return false;
	}
	if ((*sh).getPosition().x > (*player).getPosition().x + (*player).getSize().x ||
		(*sh).getPosition().x + (*sh).getSize().x < (*player).getPosition().x) {
		return false;
	}
	return true;
}

bool collisions(ArrayList* arr, sf::RectangleShape* player) {
	for (int i = 0; i < (*arr).getLength(); i++) {
		if (collision((*arr).get(i), player)) {
			return true;
		}
	}
	return false;
}






int main() {
	

	sf::RenderWindow window(sf::VideoMode(1600, 900), "SFML works!", sf::Style::Default);
	window.setFramerateLimit(60);
	window.clear(sf::Color::Black);
	short r = 0, g = 0, b = 0;
	int rx = 0, ry = 0, cx = 0, cy = 0;

	ArrayList arr = ArrayList(100);
	sf::Vector2f bullet = sf::Vector2f(10, 20);
	sf::RectangleShape player = sf::RectangleShape(sf::Vector2f(130, 80));
	player.setFillColor(sf::Color::Green);
	player.setPosition(sf::Vector2f(750, 850));
	
	int timer = 0;
	while (window.isOpen()) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			rx -= 5;
			
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			rx += 5;
		}
		player.setPosition(sf::Vector2f(player.getPosition().x + rx, player.getPosition().y));
		/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			ry+=3;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			ry-=3;
		}
		*/
		rx = 0;
		ry = 0;

		arr.move();
		if (collisions(&arr, &player)) {
			window.clear(sf::Color::Red);
			sf::Text text;
			text.setString("POMER");
			text.setFillColor(sf::Color::White);
			text.setCharacterSize(50);
			text.setPosition(200, 200);
			window.draw(text);
			break;
		}
		drawAll(&player, &arr, &window);
		
		timer++;
		if (timer > 20) {
			sf::RectangleShape* sh = new sf::RectangleShape(bullet);
			(*sh).setPosition(sf::Vector2f(rand() % 1000, -100));
			(*sh).setFillColor(sf::Color::White);
			arr.push(sh, rand() % 4);
			clear(&arr);
			timer = 0;
		}
		window.display();
	}

	window.display();
	system("pause");
	return 0;
}
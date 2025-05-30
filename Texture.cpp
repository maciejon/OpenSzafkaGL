#include"Texture.h"
#include <vector>

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	type = texType;

	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);
	if (!bytes)
	{
		std::cerr << "Failed to load texture: " << image << std::endl;
		return;
	}


	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(texType, ID);


	/*Wsp�rz�dne tekstury zazwyczaj mieszcz� si� w zakresie od (0,0) do (1,1), ale
   co si� stanie, je�li podamy wsp�rz�dne poza ten zakres? Domy�lne
	zachowanie OpenGL polega na powtarzaniu obraz�w tekstury (w zasadzie ignorujemy
   cz�� ca�kowit� zmiennoprzecinkowych wsp�rz�dnych tekstury), ale
	istnieje wi�cej opcji oferowanych przez OpenGL:*/

	/*GL_REPEAT: Domy�lne zachowanie dla tekstur. Powtarza obraz tekstury.
	GL_MIRRORED_REPEAT: To samo co GL_REPEAT, ale odbija obraz przy ka�dym
   powt�rzeniu.
	GL_CLAMP_TO_EDGE: Ogranicza wsp�rz�dne mi�dzy 0 a 1. Skutkiem tego jest to, �e
   wy�sze wsp�rz�dne s� ograniczane do kraw�dzi.
	GL_CLAMP_TO_BORDER: Wsp�rz�dne poza zakresem otrzymuj� teraz okre�lony przez
   u�ytkownika kolor obramowania.*/

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	/*Filtrowanie tekstur mo�e by� ustawione dla operacji powi�kszania i
   pomniejszania (przy zmianie skali w g�r� lub w d�), dzi�ki czemu na przyk�ad
	mo�na u�y� filtrowania s�siad�w najbli�szych, gdy tekstury s� zmniejszane, oraz
   filtrowania liniowego dla tekstur powi�kszonych. Dlatego te� musimy
	okre�li� metod� filtrowania dla obu opcji za pomoc� glTexParameter*/
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Dodatkowe linie dla GL_CLAMP_TO_BORDER 
	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor); 

	GLenum actualFormat;
	switch (numColCh) {
	case 1: actualFormat = GL_RED; break;
	case 3: actualFormat = GL_RGB; break;
	case 4: actualFormat = GL_RGBA; break;
	default:
		std::cerr << "Unsupported number of color channels: " << numColCh << std::endl;
		stbi_image_free(bytes);
		return;
	}
	
	glTexImage2D(texType, 0, actualFormat, widthImg, heightImg, 0, actualFormat, pixelType, bytes);
	// Mipmapy 
	/*GL_NEAREST_MIPMAP_NEAREST: wybiera najbli�szy poziom mipmapy pasuj�cy do
   rozmiaru piksela i u�ywa interpolacji najbli�szych s�siad�w do pr�bkowania tekstury.
	GL_LINEAR_MIPMAP_NEAREST: wybiera najbli�szy poziom mipmapy i pr�bkuje ten
   poziom za pomoc� interpolacji liniowej.
	GL_NEAREST_MIPMAP_LINEAR: interpoluje liniowo mi�dzy dwoma mipmapami, kt�re
   najbardziej odpowiadaj� rozmiarowi piksela, i pr�bkuje
	poziom interpolowany za pomoc� interpolacji najbli�szych s�siad�w.
	GL_LINEAR_MIPMAP_LINEAR: interpoluje liniowo mi�dzy dwoma najbli�szymi mipmapami
   i pr�bkuje poziom interpolowany za pomoc� interpolacji liniowej.*/
	glGenerateMipmap(texType);

	stbi_image_free(bytes);

	glBindTexture(texType, 0);
}
GLuint Texture::loadCubemap(std::vector<std::string> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			GLenum format = GL_RGB;
			if (nrChannels == 1) format = GL_RED;
			else if (nrChannels == 3) format = GL_RGB;
			else if (nrChannels == 4) format = GL_RGBA;

			// GL_TEXTURE_CUBE_MAP_POSITIVE_X + i działa, bo te enumy są kolejne
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data); // Nawet jeśli data jest nullptr, to bezpieczne
			glDeleteTextures(1, &textureID); // Zwolnij zasób, jeśli się nie udało
			return 0; // Zwróć 0 w przypadku błędu
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0); // Unbind
	return textureID;
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(texUni, unit);
}


void Texture::Bind()
{
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}
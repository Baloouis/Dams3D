#include"Camera.h"



Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform)
{
	//Transformation matrices init ( to identity matrices and not null )
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);


	// Assigns different transformations to each matrix :
	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Adds perspective to the scene
	float screenAspectRatio = (float)(width / height);
	projection = glm::perspective(glm::radians(FOVdeg), screenAspectRatio, nearPlane, farPlane);


	//Setting matrixes uniforms variables, to be used later in the vertex shader
	//NB : get reference to (i.e. get ID of ) uniform variable "camMatrix" used in the vertex shader. A uniform variable is a kind of universal/global variable accessible from everywhere
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}



void Camera::Inputs(GLFWwindow* window)
{
	//// HANDLE TRANSLATIONS key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 0.4f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 0.1f;
	}


	//// HANDLE ROTATION mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the mouse cursor such that the origin is in the middle of the screen (instead of starting from the top left corner)
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation ( we do this axis before to prevent barrel rolling with the camera -> see if check below )
		//NB: notice we only rotate on x axis, and later on the z/up axis. we don't do y-axis rotation at this would just get confusing for th user.
		glm::vec3 newOrientationX = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		// Decides whether or not the next vertical Orientation is legal or not ( if angle between cam rot and world-up is not too big) 
		if (abs(glm::angle(newOrientationX, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientationX;
		}

		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
}
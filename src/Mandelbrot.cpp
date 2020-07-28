#include <Jolt.h>
using namespace Jolt;

class MandelbrotLayer : public Layer
{
public:
	MandelbrotLayer()
		:Layer("Mandelbrot"), m_Offset(0.0f, 0.0f), m_Scale(0.001f), m_Exp(-3.0f), m_Degree(2.0f), m_FirstMouse(true)
	{

	}

	virtual void OnAttach() override
	{
		m_Shader = CreateUnique<Shader>("src/shader.glsl");
		m_Shader->Bind();
		m_Shader->SetFloat("u_Scale", m_Scale);
		m_Shader->SetVec2("u_Offset", m_Offset);
		m_Shader->SetFloat("u_Degree", m_Degree);

		int width, height;
		glfwGetFramebufferSize(Application::Get().GetWindow()->GetNaitiveWindow(), &width, &height);
		m_Shader->SetVec2("u_WindowSize", width, height);

		RectangleBuilder builder{};
		m_FlatRectangle = std::move(*builder.GenerateMesh());

	}

	virtual void OnDetach() override
	{

	}

	virtual void OnUpdate(float ts) override
	{
		JOLT_PROFILE_FUNCTION();

		m_FlatRectangle.m_VertexArray.Bind();
		m_Shader->Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	virtual void OnImGuiRender() override
	{
		JOLT_PROFILE_FUNCTION();
		ImGui::Text("Scale: %.3f", m_Exp);

		if (ImGui::SliderFloat("Degree", &m_Degree, 0.001f, 10.0f))
		{
			m_Shader->SetFloat("u_Degree", m_Degree);
		}

		if (ImGui::Button("Reset Degree"))
		{
			m_Degree = 2.0f;
			m_Shader->SetFloat("u_Degree", m_Degree);
		}
	}

	virtual void OnEvent(Event* e) override
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseMovedEvent>(JOLT_BIND_EVENT_FN(MandelbrotLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(JOLT_BIND_EVENT_FN(MandelbrotLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<WindowResizeEvent>(JOLT_BIND_EVENT_FN(MandelbrotLayer::OnWindowResizeEvent));
	}

private:
	std::unique_ptr<Shader> m_Shader;
	Mesh m_FlatRectangle;
	glm::vec2 m_Offset;
	float m_Scale, m_Exp, m_Degree;

	bool m_FirstMouse;
	glm::vec2 last_pos;

	bool OnMouseMovedEvent(MouseMovedEvent& e)
	{
		if (!Input::Get().IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1) || ImGui::GetIO().WantCaptureMouse)
		{
			m_FirstMouse = true;
			return false;
		}

		glm::vec2 current_pos = glm::vec2(e.GetX(), -e.GetY()); // Flip y for some reason
		if (m_FirstMouse)
		{
			last_pos = current_pos;

			m_FirstMouse = false;
		}

		m_Offset += m_Scale * (last_pos - current_pos);
		last_pos = current_pos;

		m_Shader->SetVec2("u_Offset", m_Offset);


		return true;

	}

	bool OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		m_Exp += e.GetYOffset() * 0.05f;
		m_Scale = glm::pow(10, m_Exp);
		m_Shader->SetFloat("u_Scale", m_Scale);

		return false;
	}

	bool OnWindowResizeEvent(WindowResizeEvent& e)
	{
		m_Shader->SetVec2("u_WindowSize", e.GetWidth(), e.GetHeight());
		return false;
	}
};

class MandelbrotApp : public Application
{
public:
	MandelbrotApp()
		:Application("Mandelbrot")
	{
		PushLayer(new MandelbrotLayer());
	}
};


int main()
{
	MandelbrotApp* app = new MandelbrotApp();
	app->Run();

	return 0;
}
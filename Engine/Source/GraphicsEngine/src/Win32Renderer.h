//  *******************************************************************************************************************
//  Win32Render   version:  1.0   Ankur Sheel  date: 2015/06/03
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef WIN32RENDER_H__
#define WIN32RENDER_H__

namespace Graphics
{
	struct stInitializationParameters;
}

namespace Graphics
{
	class cWin32Renderer
	{
	public:
		cWin32Renderer();
		~cWin32Renderer();
		bool Initialize(const stInitializationParameters& params);
		void BeginRender();
		void EndRender();
		void SetFullScreenMode(const bool isFullScreen);
		Base::tOptional<bool> OnWindowResized(const int width, const int height);
		void Cleanup();
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Creates and returns a pointer to a singleton object of this interface
		///
		/// @return Returns a pointer to the singleton object
		///
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		static const std::unique_ptr<cWin32Renderer>& Instance();
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Deletes the singleton object
		///
		///
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		static void Destroy();

	private:
		static std::unique_ptr<cWin32Renderer> s_pWin32Renderer;  ///< static object of this class

		class cWin32RendererImpl;
		std::unique_ptr<cWin32RendererImpl> m_pImpl;
	};
}  // namespace Graphics
#endif  // WIN32RENDER_H__

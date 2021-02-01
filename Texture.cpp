#include "Texture.h"
//TODO lowercase r 
void RenderTextureID(TextureID* tid, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip){
	
	//set the renderquad to the whole texture
	SDL_Rect renderQuad = { x, y, tid->src.w, tid->src.h };

	//Set clip rendering dimensions if any
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	//TODO chekc for null values in the graphics manager
	SDL_RenderCopyEx(Renderer, GraphicsManager[tid->name], clip, &renderQuad, angle, center, flip);
	//SDL_Texture* temp = GraphicsManager[text->name];
	
}



void setTextureID(TextureID* texture, std::string name) {
	SDL_Texture* query = GraphicsManager[name];
	
	if (query == NULL) {
		printf("Texture name: %s not found in asset manager!\n", name.c_str());
	}
	else {
		texture->name = name;
		SDL_QueryTexture(query, NULL, NULL, &texture->src.w, &texture->src.h);
	}

	query = NULL;

}
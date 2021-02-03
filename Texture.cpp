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

void RenderTextureID(TextureID* tid, glm::vec2 pos, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {

	//set the renderquad to the whole texture
	SDL_Rect renderQuad = {(int)(pos[0] - tid->src.w/2), (int)(pos[1] - tid->src.h/2), tid->src.w, tid->src.h };
	//printf("renderQuad: (%d,%d,%d,%d) \n", renderQuad.x ,renderQuad.y,renderQuad.w, renderQuad.h);


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
namespace mage {
template<class T, class LoaderArchive>
inline resourceCereal<T, LoaderArchive>::resourceCereal()
{
}
template<class T, class LoaderArchive>
inline resourceCereal<T, LoaderArchive>::resourceCereal(std::string filepath)
	: resourceFilepath(filepath)
{
}
template<class T, class LoaderArchive>
inline bool resourceCereal<T, LoaderArchive>::load()
{
	return s::deserializeFile<LoaderArchive, T>(getFilepath(), data);
}
template<class T, class LoaderArchive>
inline void resourceCereal<T, LoaderArchive>::unload()
{
	data = T();
}
template<class T, class LoaderArchive>
inline T * resourceCereal<T, LoaderArchive>::get()
{
	return &data;
}
template<class T, class LoaderArchive>
inline bool resourceCereal<T, LoaderArchive>::isNeeded()
{
	return numReferences() > 0;
}
}
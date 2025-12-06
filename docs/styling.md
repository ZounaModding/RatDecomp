# Styling

## Variable

- Local variables should begin with `l_`
  - `l_BitCpt`, `l_BitIndex`
- In function headers, input parameters should start with `i_` and output parameters should start with `o_`. If a parameter is used as both input and output it should start with `io_`.
  - `void Bitmap_Z::InitBmap(S32 i_SizeX, S32 i_SizeY, U8 i_Format, U8* i_Datas, U8* i_Palette)`
- In classes, member variables should begin with `m_`
  - `m_SizeX`, `m_Flag`, `m_TrueFormat`
- When putting the actual name of the variable, the names should be prefix_CapitalLetters, i.e. `m_SizeX` (seen in Bitmap_Z)

## Tabbing

- Tabbing is typical 4 spaces in size. This is not a hard requirement however as we do plan on adding an autoformatter.

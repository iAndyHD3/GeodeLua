import pefile

import pefile
import requests

def dump_symbols(dll_path):
    pe = pefile.PE(dll_path)
    symbols = []
    if hasattr(pe, 'DIRECTORY_ENTRY_EXPORT'):
        for exp in pe.DIRECTORY_ENTRY_EXPORT.symbols:
            symbols.append(exp.name.decode('utf-8'))
    return symbols

def demangle_symbols(symbols):
    # Make a POST request to the demangler.com API
    url = 'http://demangler.com/raw'
    data = {'input': '\n'.join(symbols)}  # Join symbols with newline
    response = requests.post(url, data=data)
    if response.status_code == 200:
        return response.text.strip().split('\n')  # Split demangled symbols by newline
    else:
        return ["Failed to demangle symbol"] * len(symbols)
    

if __name__ == "__main__":
    dll_path = "PATH/libcocos2d.dll"
    symbols = dump_symbols(dll_path)
    demangled_symbols = demangle_symbols(symbols)
    for original, demangled in zip(symbols, demangled_symbols):
        print(demangled)

/* Reverse Engineer's Hex Editor
 * Copyright (C) 2018-2019 Daniel Collins <solemnwarning@solemnwarning.net>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef REHEX_NUMERICENTRYDIALOG_HPP
#define REHEX_NUMERICENTRYDIALOG_HPP

#include <sstream>
#include <string>
#include <type_traits>
#include <wx/dialog.h>
#include <wx/stattext.h>

#include "NumericTextCtrl.hpp"

namespace REHex {
	template<typename T> class NumericEntryDialog: public wxDialog
	{
		private:
			const T min_value;
			const T max_value;
			const T rel_base;
			
			NumericTextCtrl *textbox;
			
		public:
			NumericEntryDialog(wxWindow *parent, const std::string &title, const std::string &text, T initial_value, T min_value = std::numeric_limits<T>::min(), T max_value = std::numeric_limits<T>::max(), T rel_base = 0):
				wxDialog(parent, wxID_ANY, title),
				min_value(min_value),
				max_value(max_value),
				rel_base(rel_base)
			{
				wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
				
				wxStaticText *st = new wxStaticText(this, wxID_ANY, text);
				topsizer->Add(st, 1, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 10);
				
				std::ostringstream ss;
				ss << initial_value;
				std::string initial_text = ss.str();
				
				textbox = new NumericTextCtrl(this, wxID_ANY, initial_text);
				topsizer->Add(textbox, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
				
				wxBoxSizer *button_sizer = new wxBoxSizer(wxHORIZONTAL);
				
				wxButton *ok     = new wxButton(this, wxID_OK,     "OK");
				wxButton *cancel = new wxButton(this, wxID_CANCEL, "Cancel");
				
				button_sizer->Add(ok,     0, wxALL, 10);
				button_sizer->Add(cancel, 0, wxALL, 10);
				
				topsizer->Add(button_sizer, 0, wxALIGN_RIGHT);
				
				SetSizerAndFit(topsizer);
				
				/* Trigger the "OK" button if enter is pressed. */
				ok->SetDefault();
				
				ok->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this](wxCommandEvent &event)
				{
					try {
						GetValue();
					}
					catch(const REHex::NumericTextCtrl::InputError &e)
					{
						wxMessageBox(e.what(), "Error", (wxOK | wxICON_EXCLAMATION | wxCENTRE), this);
						return;
					}
					
					/* Continue on to handling in base class. */
					event.Skip();
				});
			}
			
			T GetValue()
			{
				return textbox->GetValue(min_value, max_value, rel_base);
			}
	};
}

#endif /* !REHEX_NUMERICENTRYDIALOG_HPP */

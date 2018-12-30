#ifndef __FILEDIALOGUE_H__
#define __FILEDIALOGUE_H__

#define MAX_FILE_SIZE 250

enum FileDialogueState
{
	FDS_OPEN = 0,
	FDS_CLOSE
};

class FileDialogue
{
public:
	FileDialogue();
	virtual ~FileDialogue();

	void Open() { m_state = FileDialogueState::FDS_OPEN; }
	void Close() { m_state = FileDialogueState::FDS_CLOSE; m_shouldClose = false; m_selectedFile[0] = '\0'; }
	bool IsOpen()const { return m_state == FileDialogueState::FDS_OPEN; }
	bool IsClosed()const { return m_state == FileDialogueState::FDS_CLOSE; }
	FileDialogueState GetState()const { return m_state; }

	const char* GetSelectedFile()const { return m_selectedFile; }
	bool ValidFileSelected() { return m_selectedFile[0] != '\0'; }
	bool Display(const char* dir, const char* filterExt);

private:
	void Directories(const char* dir, const char* filterExt);

private:
	char m_selectedFile[MAX_FILE_SIZE];
	bool m_shouldClose;
	FileDialogueState m_state = FileDialogueState::FDS_CLOSE;
};

#endif // !__FILEDIALOGUE_H__
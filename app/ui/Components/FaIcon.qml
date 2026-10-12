import QtQuick

// IconFonts 是由 C++ 注册的 singleton，确保所有图标使用同一组字体。
Text {
    id: root

    property bool solid: true

    font.family: root.solid ? IconFonts.solid : IconFonts.regular
    font.styleName: root.solid ? IconFonts.solidStyle : IconFonts.regularStyle
}

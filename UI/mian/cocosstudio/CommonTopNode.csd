<GameFile>
  <PropertyGroup Name="CommonTopNode" Type="Node" ID="1bc235b7-99a6-4b2c-8611-91d2385d09cf" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000">
        <Timeline ActionTag="-980505132" Property="Position">
          <PointFrame FrameIndex="0" X="117.8681" Y="17.0507">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-980505132" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.9000" Y="0.9000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-980505132" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-980505132" Property="FileData">
          <TextureFrame FrameIndex="0" Tween="False">
            <TextureFile Type="MarkedSubImage" Path="UI/gold.png" Plist="ui.plist" />
          </TextureFrame>
        </Timeline>
      </Animation>
      <ObjectData Name="Node" Tag="169" ctype="GameNodeObjectData">
        <Size X="0.0000" Y="0.0000" />
        <Children>
          <AbstractNodeData Name="numBgHp" ActionTag="139273362" Tag="183" IconVisible="False" LeftMargin="-255.8566" RightMargin="102.8566" TopMargin="-41.5686" BottomMargin="-4.4314" ctype="SpriteObjectData">
            <Size X="153.0000" Y="46.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="-179.3566" Y="18.5686" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="MarkedSubImage" Path="UI/numBg.png" Plist="ui.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="hp" ActionTag="627883941" Tag="182" IconVisible="False" LeftMargin="-271.6845" RightMargin="213.6845" TopMargin="-45.0652" BottomMargin="-7.9348" ctype="SpriteObjectData">
            <Size X="58.0000" Y="53.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="-242.6845" Y="18.5652" />
            <Scale ScaleX="0.9000" ScaleY="0.9000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="MarkedSubImage" Path="UI/hp.png" Plist="ui.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="hpbmlbl" ActionTag="-694847476" Tag="181" IconVisible="False" LeftMargin="-205.5278" RightMargin="147.5278" TopMargin="-27.5672" BottomMargin="1.5672" LabelText="25/25" ctype="TextBMFontObjectData">
            <Size X="58.0000" Y="26.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="-176.5278" Y="14.5672" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <LabelBMFontFile_CNB Type="Normal" Path="fonts/commonnum.fnt" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="HpBtn" ActionTag="-202855615" Tag="180" IconVisible="False" LeftMargin="-139.0986" RightMargin="96.0986" TopMargin="-41.5680" BottomMargin="-0.4320" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="13" Scale9Height="20" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="ButtonObjectData">
            <Size X="43.0000" Y="42.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="-117.5986" Y="20.5680" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="MarkedSubImage" Path="UI/buy.png" Plist="ui.plist" />
            <PressedFileData Type="MarkedSubImage" Path="UI/buy.png" Plist="ui.plist" />
            <NormalFileData Type="MarkedSubImage" Path="UI/buy.png" Plist="ui.plist" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="numBgDiamond" ActionTag="1448860994" Tag="179" IconVisible="False" LeftMargin="-70.9319" RightMargin="-82.0681" TopMargin="-42.0299" BottomMargin="-3.9701" ctype="SpriteObjectData">
            <Size X="153.0000" Y="46.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="5.5681" Y="19.0299" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="MarkedSubImage" Path="UI/numBg.png" Plist="ui.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="diamond" ActionTag="1354458053" Tag="178" IconVisible="False" LeftMargin="-96.7876" RightMargin="36.7876" TopMargin="-44.5291" BottomMargin="-10.4709" ctype="SpriteObjectData">
            <Size X="60.0000" Y="55.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="-66.7876" Y="17.0291" />
            <Scale ScaleX="0.9000" ScaleY="0.9000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="MarkedSubImage" Path="UI/diamond.png" Plist="ui.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="gembmlbl" ActionTag="1194034297" Tag="173" IconVisible="False" LeftMargin="-4.1326" RightMargin="-7.8674" TopMargin="-27.6942" BottomMargin="1.6942" LabelText="0" ctype="TextBMFontObjectData">
            <Size X="12.0000" Y="26.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="1.8674" Y="14.6942" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <LabelBMFontFile_CNB Type="Normal" Path="fonts/commonnum.fnt" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="diamondBtn" ActionTag="2089839936" Tag="176" IconVisible="False" LeftMargin="45.2394" RightMargin="-88.2394" TopMargin="-41.0355" BottomMargin="-0.9645" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="13" Scale9Height="20" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="ButtonObjectData">
            <Size X="43.0000" Y="42.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="66.7394" Y="20.0355" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="MarkedSubImage" Path="UI/buy.png" Plist="ui.plist" />
            <PressedFileData Type="MarkedSubImage" Path="UI/buy.png" Plist="ui.plist" />
            <NormalFileData Type="MarkedSubImage" Path="UI/buy.png" Plist="ui.plist" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="numBgGold" ActionTag="1122973732" Tag="175" IconVisible="False" LeftMargin="109.4759" RightMargin="-262.4759" TopMargin="-40.0554" BottomMargin="-5.9446" ctype="SpriteObjectData">
            <Size X="153.0000" Y="46.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="185.9759" Y="17.0554" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="MarkedSubImage" Path="UI/numBg.png" Plist="ui.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="gold" ActionTag="-980505132" Tag="174" IconVisible="False" LeftMargin="88.3681" RightMargin="-147.3681" TopMargin="-46.5507" BottomMargin="-12.4493" ctype="SpriteObjectData">
            <Size X="59.0000" Y="59.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="117.8681" Y="17.0507" />
            <Scale ScaleX="0.9000" ScaleY="0.9000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="MarkedSubImage" Path="UI/gold.png" Plist="ui.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="goldbmlbl" ActionTag="-65651828" Tag="177" IconVisible="False" LeftMargin="162.5374" RightMargin="-211.5374" TopMargin="-25.5080" BottomMargin="-0.4920" LabelText="2000" ctype="TextBMFontObjectData">
            <Size X="49.0000" Y="26.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="187.0374" Y="12.5080" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <LabelBMFontFile_CNB Type="Normal" Path="fonts/commonnum.fnt" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="goldBtn" ActionTag="-1086024609" Tag="172" IconVisible="False" LeftMargin="226.1288" RightMargin="-269.1288" TopMargin="-39.0537" BottomMargin="-2.9463" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="13" Scale9Height="20" OutlineSize="0" ShadowOffsetX="0.0000" ShadowOffsetY="0.0000" ctype="ButtonObjectData">
            <Size X="43.0000" Y="42.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="247.6288" Y="18.0537" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="MarkedSubImage" Path="UI/buy.png" Plist="ui.plist" />
            <PressedFileData Type="MarkedSubImage" Path="UI/buy.png" Plist="ui.plist" />
            <NormalFileData Type="MarkedSubImage" Path="UI/buy.png" Plist="ui.plist" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="timebg" ActionTag="650654439" Tag="171" IconVisible="False" LeftMargin="-206.0264" RightMargin="133.0264" TopMargin="7.4345" BottomMargin="-29.4345" ctype="SpriteObjectData">
            <Size X="73.0000" Y="22.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="-169.5264" Y="-18.4345" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="MarkedSubImage" Path="UI/timebg.png" Plist="ui.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="timebmlbl" ActionTag="467565693" Tag="170" IconVisible="False" LeftMargin="-195.0193" RightMargin="144.0193" TopMargin="10.4362" BottomMargin="-42.4362" LabelText="10:58" ctype="TextBMFontObjectData">
            <Size X="51.0000" Y="32.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="-169.5193" Y="-26.4362" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <LabelBMFontFile_CNB Type="Normal" Path="fonts/num.fnt" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>
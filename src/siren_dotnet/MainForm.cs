/* THIS FILE IS PART OF *SIREN* SOURCE CODES.
 * WHEN YOU WANT THE LICENSE OF THIS FILE, YOU CAN GET IT
 * IN README OR LICENSE DOCUMENTATION OF THIS PROJECT.
 * ---------------------------------------------------------
 * AUTHOR: dyama <dyama@member.fsf.org>
 */

using System;
using System.Windows.Forms;
using System.Collections.Generic;
using sirenenv;

namespace siren
{
	public enum ModelFormat : int{ UNKNOWN = -1, BREP = 0, CSFDB = 1, STEP = 2, IGES = 3, VRML = 4, STL = 5, IMAGE = 6 }

	public partial class MainForm : System.Windows.Forms.Form
	{
        protected ViewForm view1;
        protected ViewForm view2;
        protected ViewForm view3;

        public MousePicking MousePickingEvent = null;

        public ObjectProperty SelectedObject = null;

		[STAThread]
		static void Main() 
		{
			Application.Run(new MainForm());
        }

        #region "�C�x���g"

        void MainForm_Load(object sender, EventArgs e)
        {
            this.view1 = new ViewForm(this, TypeOfOrientation.XnegYnegZpos, true);
            view1.Dock = DockStyle.Fill;
            splitContainer2.Panel1.Controls.Add(this.view1);
			view1.InitViewer();
			view1.InitV3D(false);

            this.view2 = new ViewForm(this, TypeOfOrientation.Zneg, false);
            view2.Dock = DockStyle.Fill;
            splitContainer4.Panel1.Controls.Add(this.view2);
            view2.InitViewer();
            view2.SetContext(view1.Viewer);
            view2.Viewer.CreateNewView(view2.Handle, false, false);
            view2.Viewer.setProjection(TypeOfOrientation.Zpos);

            this.view3 = new ViewForm(this, TypeOfOrientation.Yneg, false);
            view3.Dock = DockStyle.Fill;
            splitContainer4.Panel2.Controls.Add(this.view3);
            view3.InitViewer();
            view3.SetContext(view1.Viewer);
            view3.Viewer.CreateNewView(view3.Handle, false, false);
            view3.Viewer.setProjection(TypeOfOrientation.Ypos);

            splitContainer3.Visible = false;

            List<string> args = new List<string>(System.Environment.GetCommandLineArgs());
            // �R�}���h���C����������J��
            args.RemoveAt(0); // ���g�̌Ăяo���p�X
            if (args != null && args.Count > 0) {
                foreach (string file in args) {
                    if (!OpenFile(file, Common.getFormatByExt(file)))
                        break;
                }
            }
            else {
            }

            // dummy
            propertyGrid1.SelectedObject = this;

            myTerm.set(view1.Viewer, this);
            changeState();
            myTerm.setChangeStateFunc(changeState);
            this.Cursor = System.Windows.Forms.Cursors.Default;

            view1.Viewer.RedrawView();
        }

        void MainForm_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.Move;
        }

        void MainForm_DragDrop(object sender, DragEventArgs e)
        {
            List<string> files = new List<string>((string[])e.Data.GetData(DataFormats.FileDrop, false));
            if (files != null) {
                foreach (string file in files) {
                    if (!OpenFile(file, Common.getFormatByExt(file)))
                        break;
                }
            }
        }

        #endregion // �C�x���g

        private bool OpenFile(string filename, ModelFormat theformat)
        {
            switch (theformat) {
            case ModelFormat.IMAGE:
            case ModelFormat.STL:
            case ModelFormat.VRML:
            case ModelFormat.UNKNOWN:
                MessageBox.Show("�T�|�[�g����Ă��Ȃ��t�@�C���`���ł��B", "�G���[", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

			this.Cursor = System.Windows.Forms.Cursors.WaitCursor;

            bool result = this.view1.Import(filename, theformat);

			this.Cursor = System.Windows.Forms.Cursors.Default;

            if (!result) {
                MessageBox.Show("�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B", "�G���[", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            return true;
        }

		private bool OpenFile()
		{
			OpenFileDialog d = new OpenFileDialog();
            d.CheckFileExists = true;
   
			string filter="";
            d.RestoreDirectory = true;

            filter = "�Ή����Ă���S�Ẵt�@�C��(brep brp ble csfdb stp step igs iges)|*.brep; *.brp; *.rle; *.csfdb; *.stp; *.step; *.igs; *.iges";
			filter += "|BREP �t�@�C��(*.brep *.brp *.rle)|*.brep; *.brp; *.rle";
			filter += "|CSFDB �t�@�C��(*.csfdb)|*.csfdb";
			filter += "|STEP �t�@�C��(*.stp *.step)|*.stp; *.step";
			filter += "|IGES �t�@�C��(*.igs *.iges)|*.igs; *.iges";
			d.Filter = filter+"|All files (*.*)|*.*" ;

            if (d.ShowDialog() != DialogResult.OK)
                return false;

            return OpenFile(d.FileName, Common.getFormatByExt(d.FileName));
		}

        private bool SaveFile(string filename, ModelFormat format)
        {
            switch (format) {
            case ModelFormat.UNKNOWN:
            case ModelFormat.CSFDB:
                MessageBox.Show("�T�|�[�g����Ă��Ȃ��t�@�C���`���ł��B", "�G���[", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

			this.Cursor=System.Windows.Forms.Cursors.WaitCursor;

            if (this.view1 == null)
                return false;

            bool result = this.view1.Export(filename, format);

			this.Cursor=System.Windows.Forms.Cursors.Default;

            if (!result) {
                MessageBox.Show("�t�@�C���̕ۑ��Ɏ��s���܂����B", "�G���[", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            return result;
        }

        private bool SaveFile()
        {
			System.Windows.Forms.SaveFileDialog  d = new SaveFileDialog();
			string filter="";
			filter += "BREP �t�@�C��(*.brep *.brp *.rle)|*.brep; *.brp; *.rle";
			//filter += "|CSFDB �t�@�C��(*.csfdb)|*.csfdb";
			filter += "|STEP �t�@�C��(*.stp *.step)|*.step; *.stp";
			filter += "|IGES �t�@�C��(*.igs *.iges)| *.iges; *.igs";
			filter += "|VRML �t�@�C��(*.vrml)|*.vrml";
			filter += "|STL �t�@�C��(*.stl)|*.stl";
			//filter += "|WebGL JavaScript �t�@�C��(*.js)|*.js";
			filter += "|�摜�t�@�C��(*.bmp *.gif *.xwd)| *.bmp; *.gif; *.xwd";
			d.Filter = filter;

            if (d.ShowDialog() != DialogResult.OK)
                return false;

            return SaveFile(d.FileName, Common.getFormatByExt(d.FileName));
        }

		private void SaveFile_Click(object sender, System.EventArgs e)
		{
            SaveFile();
		}

        private void tsbTransparency_Click(object sender, EventArgs e)
        {
			if (this.view1 == null)
				return;
			siren.TransparencyDialog t = new TransparencyDialog();
			t.Viewer=this.view1.Viewer;
			t.ShowDialog(this.view1);
        }

        private void tsbFit_Click(object sender, EventArgs e)
        {
            if (this.view1 == null)
                return;
            this.view1.Viewer.ZoomAllView();
        }

        #region "Make premitive"

        private void tsbVertex_Click(object sender, EventArgs e)
        {
            if (this.view1 == null)
                return;

            ToolStripButton button = (ToolStripButton)sender;
            button.Checked = true;

            // �J�E���^��1�_�s�b�N�ɐݒ�
            this.MousePickingEvent = new MousePicking();
            this.MousePickingEvent.Count = 1;

            // �m�莞�C�x���g
            this.MousePickingEvent.ApplyEvent = (List<Point3d> pts) => {
                double x, y, z;
                x = pts[0].X;
                y = pts[0].Y;
                z = pts[0].Z;
                string pos = "" + x.ToString() + "," + y.ToString() + "," + z.ToString() + "";
                myTerm.execute("a = vertex " + pos);
                button.Checked = false;
            };

            // �L�����Z�����C�x���g
            this.MousePickingEvent.CancelEvent = () => {
                button.Checked = false;
            };

            return;
        }

        private void tsbLine_Click(object sender, EventArgs e)
        {
            if (this.view1 == null)
                return;

            ToolStripButton button = (ToolStripButton)sender;
            button.Checked = true;

            // �J�E���^��2�_�s�b�N�ɐݒ�
            this.MousePickingEvent = new MousePicking();
            this.MousePickingEvent.Count = 2;

            // �m�莞�C�x���g
            this.MousePickingEvent.ApplyEvent = (List<Point3d> pts) => {
                string spos = pts[0].to_a();
                string tpos = pts[1].to_a();
                myTerm.execute("a = line " + spos + ", " + tpos);
                button.Checked = false;
            };

            // �L�����Z�����C�x���g
            this.MousePickingEvent.CancelEvent = () => {
                button.Checked = false;
            };

            return;
        }

        private void tsbPolyline_Click(object sender, EventArgs e)
        {
            if (this.view1 == null)
                return;

            ToolStripButton button = (ToolStripButton)sender;
            button.Checked = true;

            // �J�E���^������Ȃ��ɐݒ�
            this.MousePickingEvent = new MousePicking();
            this.MousePickingEvent.Count = -1;

            // �m�莞�C�x���g
            this.MousePickingEvent.ApplyEvent = (List<Point3d> pts) => {
                string args = string.Empty;
                foreach (Point3d p in pts) {
                    args += (args.Length > 0 ? ", " : "") + p.to_a();
                }
                myTerm.execute("a = polyline [" + args + "]");
                button.Checked = false;
            };

            // �L�����Z�����C�x���g
            this.MousePickingEvent.CancelEvent = () => {
                button.Checked = false;
            };

            return;
        }

        private void tsbCurve_Click(object sender, EventArgs e)
        {
            if (this.view1 == null)
                return;

            ToolStripButton button = (ToolStripButton)sender;
            button.Checked = true;

            // �J�E���^������Ȃ��ɐݒ�
            this.MousePickingEvent = new MousePicking();
            this.MousePickingEvent.Count = -1;

            // �m�莞�C�x���g
            this.MousePickingEvent.ApplyEvent = (List<Point3d> pts) => {
                string args = string.Empty;
                foreach (Point3d p in pts) {
                    args += (args.Length > 0 ? ", " : "") + p.to_a();
                }
                myTerm.execute("a = curve [" + args + "]");
                button.Checked = false;
            };

            // �L�����Z�����C�x���g
            this.MousePickingEvent.CancelEvent = () => {
                button.Checked = false;
            };

            return;
        }

        private void tsbPlane_Click(object sender, EventArgs e)
        {
            if (this.view1 == null)
                return;
            myTerm.execute("a = plane [0, 0, 0], [0, 0, 1], -10, 10, -10, 10");
        }

        private void tsbBox_Click(object sender, EventArgs e)
        {
            if (this.view1 == null)
                return;
            myTerm.execute("a = box [10,10,10]");
        }

        private void tsbSphere_Click(object sender, EventArgs e)
        {
            if (this.view1 == null)
                return;
            myTerm.execute("a = sphere 10");
        }

        private void tsbCylinder_Click(object sender, EventArgs e)
        {
            if (this.view1 == null)
                return;
            myTerm.execute("a = cylinder [0, 0, 0], [0, 0, 1], 10, 20, " + deg2rad(360).ToString());
        }

        private void tsbCone_Click(object sender, EventArgs e)
        {
            if (this.view1 == null)
                return;
            myTerm.execute("a = cone [0, 0, 0], [0, 0, 1], 10, 0, 20, " + deg2rad(360).ToString() );
        }

        private void tsbTorus_Click(object sender, EventArgs e)
        {
            if (this.view1 == null)
                return;
            string a = deg2rad(360).ToString();
            myTerm.execute("a = torus([0, 0, 0], [0, 0, 1], 7, 3, "+a+")");
        }

        private double deg2rad(double deg)
        {
            return (Math.PI / 180.0 * deg);
        }

        #endregion

        private void tsbExit_Click(object sender, EventArgs e)
        {
            this.Close();
            return;
        }

        private void tsbSave_Click(object sender, EventArgs e)
        {
            this.SaveFile();
        }

        private void tsbOpen_Click(object sender, EventArgs e)
        {
            this.OpenFile();
        }

        private void tsbDump_Click(object sender, EventArgs e)
        {
            SaveFileDialog d = new SaveFileDialog();
            d.RestoreDirectory = true;

            string filter = "";
			filter += "�摜�t�@�C��(*.bmp *.gif *.xwd)| *.bmp; *.gif; *.xwd";
			filter += "|BMP �t�@�C��(*.bmp)|*.bmp";
			filter += "|GIF �t�@�C��(*.gif)|*.gif";
			filter += "|XWD �t�@�C��(*.xwd)|*.xwd";
			d.Filter = filter;

            d.Title = "�X�N���[���V���b�g�̕ۑ�";

            if (d.ShowDialog() != DialogResult.OK)
                return;

            SaveFile(d.FileName, ModelFormat.IMAGE);
        }

        private void tsbDisplayMode_Click(object sender, EventArgs e)
        {
            if (this.view1 != null) {
                this.view1.toggleDisplayMode();
            }
        }

        private void tsbDelete_Click(object sender, EventArgs e)
        {
            if (this.view1 != null) {
                this.view1.Viewer.EraseObjects();
            }
        }

        private void tsbCopy_Click(object sender, EventArgs e)
        {
            if (this.view1 != null) {
                myTerm.execute("a = []; selected.each { |obj| a.push(copy(obj)) }");
                changeState();
            }
        }

        private void tsbColor_Click(object sender, EventArgs e)
        {
            if (this.view1 == null)
                return;

            int r, g, b;
            r = this.view1.Viewer.GetObjColR();
            g = this.view1.Viewer.GetObjColG();
            b = this.view1.Viewer.GetObjColB();

            System.Windows.Forms.ColorDialog ColDlg = new ColorDialog();
            ColDlg.Color = System.Drawing.Color.FromArgb(r, g, b);
            if (ColDlg.ShowDialog() == DialogResult.OK) {
                System.Drawing.Color c = ColDlg.Color;
                r = c.R;
                g = c.G;
                b = c.B;
                this.view1.Viewer.SetColor(r, g, b);
            }
            this.view1.Viewer.UpdateCurrentViewer();
        }

        private void tsbHlr_Click(object sender, EventArgs e)
        {
            if (this.view1 == null)
                return;
            this.view1.myDegenerateModeIsOn = !this.view1.myDegenerateModeIsOn;
            this.view1.Viewer.UpdateCurrentViewer();
        }

        #region Materialize
        private void tsbMaterial_ButtonClick(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.Viewer.SetMaterial((int)NameOfMaterial.DEFAULT); } 
        private void miGold_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.Viewer.SetMaterial((int)NameOfMaterial.GOLD); } 
        private void miSilver_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.Viewer.SetMaterial((int)NameOfMaterial.SILVER); } 
        private void miCopper_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.Viewer.SetMaterial((int)NameOfMaterial.COPPER); } 
        private void miBronze_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.Viewer.SetMaterial((int)NameOfMaterial.BRONZE); } 
        private void miPewter_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.Viewer.SetMaterial((int)NameOfMaterial.PEWTER); } 
        private void miBrass_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.Viewer.SetMaterial((int)NameOfMaterial.BRASS); } 
        private void miSteel_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.Viewer.SetMaterial((int)NameOfMaterial.STEEL); } 
        private void miAluminium_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.Viewer.SetMaterial((int)NameOfMaterial.ALUMINIUM); } 
        private void miMetalized_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.Viewer.SetMaterial((int)NameOfMaterial.METALIZED); } 
        private void miStone_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.Viewer.SetMaterial((int)NameOfMaterial.STONE); } 
        private void miPlaster_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.Viewer.SetMaterial((int)NameOfMaterial.PLASTER); } 
        private void miObsidian_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.Viewer.SetMaterial((int)NameOfMaterial.OBSIDIAN); } 
        private void miJade_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.Viewer.SetMaterial((int)NameOfMaterial.JADE); } 
        private void miPlastic_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.Viewer.SetMaterial((int)NameOfMaterial.PLASTIC); } 
        private void miPlastic2_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.Viewer.SetMaterial((int)NameOfMaterial.SHINY_PLASTIC); }
        private void miSatin_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.Viewer.SetMaterial((int)NameOfMaterial.SATIN); } 
        private void miGNC_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.Viewer.SetMaterial((int)NameOfMaterial.NEON_GNC); } 
        private void miPHC_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.Viewer.SetMaterial((int)NameOfMaterial.NEON_PHC); }
        #endregion

        #region ViewDirection
        private void tsbAxoView_ButtonClick(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.AxoView(); } 
        private void miFront_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.FrontView(); } 
        private void miBack_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.BackView(); } 
        private void miLeft_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.LeftView(); } 
        private void miRight_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.RightView(); } 
        private void miTop_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.TopView(); } 
        private void miBottom_Click(object sender, EventArgs e) {  if (this.view1 == null) return; this.view1.BottomView(); }
        #endregion

        private void tsbResetView_Click(object sender, EventArgs e)
        {
            if (this.view1 == null)
                return;
            this.view1.Viewer.Reset();
            this.view1.Viewer.ZoomAllView();
        }

        public void changeState()
        {
            int nb_selected = this.view1.Viewer.NbSelected();
            setToolBarButtonState(nb_selected);
            setPropertyState(nb_selected);
        }

        /// <summary>
        /// �v���p�e�B
        /// </summary>
        public void setPropertyState(int nb_selected)
        {
            if (nb_selected == 1) {
                try {
                    this.myTerm.execute("location ?", this.view1, false, false);
                    string result = this.myTerm.result_string;
                    if (result == null || result.Length < 1)
                        return;
                    System.Text.RegularExpressions.Regex re = new System.Text.RegularExpressions.Regex(@"[\d\.\-]+");
                    System.Text.RegularExpressions.MatchCollection ma = re.Matches(result);

                    bool f = true;
                    double x = 0, y = 0, z = 0;
                    f = f && double.TryParse(ma[0].Value, out x);
                    f = f && double.TryParse(ma[1].Value, out y);
                    f = f && double.TryParse(ma[2].Value, out z);
                    if (!f)
                        return;

                    this.myTerm.execute("bndbox ?", this.view1, false, false);
                    result = this.myTerm.result_string;
                    ma = re.Matches(result);

                    double sx = 0, sy = 0, sz = 0, lx = 0, ly = 0, lz = 0;
                    f = f && double.TryParse(ma[0].Value, out sx);
                    f = f && double.TryParse(ma[1].Value, out sy);
                    f = f && double.TryParse(ma[2].Value, out sz);
                    f = f && double.TryParse(ma[3].Value, out lx);
                    f = f && double.TryParse(ma[4].Value, out ly);
                    f = f && double.TryParse(ma[5].Value, out lz);

                    this.SelectedObject = new ObjectProperty(x, y, z, sx, sy, sz, lx, ly, lz);
                    propertyGrid1.SelectedObject = this.SelectedObject;
                }
                catch {
                    propertyGrid1.SelectedObject = null;
                }
            }
            else {
                propertyGrid1.SelectedObject = null;
            }
        }

        /// <summary>
        /// �c�[���o�[�̃{�^���̏�Ԃ�K�؂ȏ�ԂɕύX����
        /// </summary>
        public void setToolBarButtonState(int nb_selected)
        {
            toolStripMain.SuspendLayout();
            toolStripMain.Enabled = false;

            // ��U�S�Ė����ɂ���
            foreach (ToolStripItem item in toolStripMain.Items) {
                item.Enabled = false;
            }

            // ��ɗL��
            tsbNew.Enabled = true;
            tsbOpen.Enabled = true;
            tsbExit.Enabled = true;

            // �r���[�A�[�����݂���ꍇ�̂ݗL��
            if (this.view1 != null) {
                tsbDump.Enabled = true;
                tsbRender.Enabled = true;
                // ----
                tsbVertex.Enabled = true;
                tsbLine.Enabled = true;
                tsbPolyline.Enabled = true;
                tsbCurve.Enabled = true;
                tsbPlane.Enabled = true;
                tsbBox.Enabled = true;
                tsbSphere.Enabled = true;
                tsbCylinder.Enabled = true;
                tsbCone.Enabled = true;
                tsbTorus.Enabled = true;
                // ----
                tsbFit.Enabled = true;
                tsbAxoView.Enabled = true;
                miFront.Enabled = true;
                miBack.Enabled = true;
                miLeft.Enabled = true;
                miRight.Enabled = true;
                miTop.Enabled = true;
                miBottom.Enabled = true;
                tsbResetView.Enabled = true;
                // ----
                tsbHlr.Enabled = true;

                // �I�u�W�F�N�g���I������Ă���ꍇ�̂ݗL��
                if (nb_selected > 0) {
                    tsbSave.Enabled = true;
                    tsbCopy.Enabled = true;
                    tsbDelete.Enabled = true;
                    // ----
                    tsbExplode.Enabled = true;
                    // ----
                    tsbTranslate.Enabled = true;
                    //tsbRotate.Enabled = true;
                    //tsbScale.Enabled = true;
                    //tsbMirror.Enabled = true;
                    // ----
                    //tsbOffset.Enabled = true;
                    // ----
                    tsbDisplayMode.Enabled = true;
                    tsbTransparency.Enabled = true;
                    tsbColor.Enabled = true;
                    // ----
                    tsbMaterial.Enabled = true;
                    miBrass.Enabled = true;
                    miBronze.Enabled = true;
                    miCopper.Enabled = true;
                    miGold.Enabled = true;
                    miPewter.Enabled = true;
                    miPlaster.Enabled = true;
                    miPlastic.Enabled = true;
                    miSilver.Enabled = true;
                    miSteel.Enabled = true;
                    miStone.Enabled = true;
                    miPlastic2.Enabled = true;
                    miSatin.Enabled = true;
                    miMetalized.Enabled = true;
                    miGNC.Enabled = true;
                    miChrome.Enabled = true;
                    miAluminium.Enabled = true;
                    miObsidian.Enabled = true;
                    miPHC.Enabled = true;
                    miJade.Enabled = true;
                }
                // �I�u�W�F�N�g�������I������Ă���ꍇ�̂ݗL��
                if (nb_selected > 1) {
                    tsbCompound.Enabled = true;
                }
                // �I�u�W�F�N�g��1�I������Ă���ꍇ�̂ݗL��
                if (nb_selected == 1) {
                    // �Ƃ肠����1�̂ݑΉ�
                    tsbInfo.Enabled = true;
                    miVolume.Enabled = true;
                    miCog.Enabled = true;
                    miBndbox.Enabled = true;
                }
                // �I�u�W�F�N�g��2�I������Ă���ꍇ�̂ݗL��
                if (nb_selected == 2) {
                    tsbFuse.Enabled = true;
                    tsbCut.Enabled = true;
                    tsbCommon.Enabled = true;
                    tsbIntersect.Enabled = true;
                }
                // �I�u�W�F�N�g��2�ȏ�I������Ă���ꍇ�̂ݗL��
                if (nb_selected >= 2) {
                    tsbLoft.Enabled = true;
                    tsbSewing.Enabled = true;
                }
            }
            toolStripMain.Enabled = true;
            toolStripMain.ResumeLayout(true);
        }

        #region "Transform commands"

        private void tsbTranslate_Click(object sender, EventArgs e)
        {
            
            if (this.view1 != null && this.view1.Viewer.IsObjectSelected())
                this.view1.isDirectTranslateMode = true;
        }

        #endregion

        #region "Grouping"

        private void tsbCompound_Click(object sender, EventArgs e)
        {
            if (this.view1 != null && this.view1.Viewer.NbSelected() > 1)
                myTerm.execute("a = compound selected");
        }

        private void miExCompound_Click(object sender, EventArgs e)
        {
            if (this.view1 != null)
                myTerm.execute("a = []; selected.each { |n| a.push(explode Stype::COMPOUND, n) }");
        }

        private void miExSolid_Click(object sender, EventArgs e)
        {
            if (this.view1 != null)
                myTerm.execute("a = []; selected.each { |n| a.push(explode Stype::SOLID, n) }");
        }

        private void miExShell_Click(object sender, EventArgs e)
        {
            if (this.view1 != null)
                myTerm.execute("a = []; selected.each { |n| a.push(explode Stype::SHELL, n) }");
        }

        private void miExFace_Click(object sender, EventArgs e)
        {
            if (this.view1 != null)
                myTerm.execute("a = []; selected.each { |n| a.push(explode Stype::FACE, n) }");
        }

        private void miExWire_Click(object sender, EventArgs e)
        {
            if (this.view1 != null)
                myTerm.execute("a = []; selected.each { |n| a.push(explode Stype::WIRE, n) }");
        }

        private void miExEdge_Click(object sender, EventArgs e)
        {
            if (this.view1 != null)
                myTerm.execute("a = []; selected.each { |n| a.push(explode Stype::EDGE, n) }");
        }

        private void miExVertex_Click(object sender, EventArgs e)
        {
            if (this.view1 != null)
                myTerm.execute("a = []; selected.each { |n| a.push(explode Stype::VERTEX, n) }");
        }

        #endregion

        #region "Wire operation"

        private void tsbLoft_Click(object sender, EventArgs e)
        {
            if (this.view1 != null)
                myTerm.execute("a = loft ??");
        }

        #endregion

        #region "Boolean Operations"

        private void tsbFuse_Click(object sender, EventArgs e)
        {
            if (this.view1 != null)
                myTerm.execute("a = fuse ??[0], ??[1]");
        }

        private void tsbCut_Click(object sender, EventArgs e)
        {
            if (this.view1 != null)
                myTerm.execute("a = cut ??[0], ??[1]");
        }

        private void tsbCommon_Click(object sender, EventArgs e)
        {
            if (this.view1 != null)
                myTerm.execute("a = common ??[0], ??[1]");
        }

        private void tsbIntersect_Click(object sender, EventArgs e)
        {
            if (this.view1 != null)
                myTerm.execute("a = intersect ??[0], ??[1]");
        }

        #endregion

        #region "Information"

        private void miVolume_Click(object sender, EventArgs e)
        {
            if (this.view1 != null)
                myTerm.execute("volume ?", "�{�����[��: ");
        }

        private void miCog_Click(object sender, EventArgs e)
        {
            if (this.view1 != null)
                myTerm.execute("cog ?", "�d�S�ʒu: ");
        }

        private void miBndbox_Click(object sender, EventArgs e)
        {
            if (this.view1 != null)
                myTerm.execute("bndbox ?", "�͈�(�ŏ��ʒu, �ő�ʒu): ");
        }

        #endregion

        private void tsbSewing_Click(object sender, EventArgs e)
        {
            if (this.view1 != null)
                myTerm.execute("a = sew ??");
        }

        /// <summary>
        /// �t���X�N���[����Ԃ�؂�ւ���
        /// </summary>
        public void toggleFullscreen()
        {
            if (view1.Focused) {
                if (splitContainer3.Visible) {
                    splitContainer3.Visible = false;
                    splitContainer2.Panel1.Controls.Add(view1);
                }
                else {
                    splitContainer3.Visible = true;
                    splitContainer3.Panel2.Controls.Add(view1);
                }
            }
            else if (view2.Focused) {
                if (splitContainer3.Visible) {
                    splitContainer3.Visible = false;
                    splitContainer2.Panel1.Controls.Add(view2);
                }
                else {
                    splitContainer3.Visible = true;
                    splitContainer4.Panel1.Controls.Add(view2);
                }
            }
            else if (view3.Focused) {
                if (splitContainer3.Visible) {
                    splitContainer3.Visible = false;
                    splitContainer2.Panel1.Controls.Add(view3);
                }
                else {
                    splitContainer3.Visible = true;
                    splitContainer4.Panel2.Controls.Add(view3);
                }
            }
            return;
        }

        /// <summary>
        /// �^�[�~�i���Ƀt�H�[�J�X���ڂ�
        /// </summary>
        public void focusTerminal()
        {
            this.myTerm.setFocus();
            return;
        }

        /// <summary>
        /// 
        /// </summary>
        public void focusViewer()
        {
            this.view1.Focus();
            return;
        }


    }

}